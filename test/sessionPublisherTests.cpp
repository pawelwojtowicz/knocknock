#include <gtest/gtest.h>
#include <CSessionPublisher.h>
#include <CSQLiteSessionPublisher.h>
#include <CConfiguration.h>
#include <CSession.h>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <filesystem>
#include <fstream>

#include <chrono>
#include <thread>


using namespace knocknock;

TEST(CSessionPublisherTests, InitializeAndShutdown)
{
  CConfiguration config;
  CSessionPublisher publisher;

  tSessionList sessions;

  EXPECT_TRUE(publisher.Initialize(config));

  publisher.PublishSessions(sessions);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for the publisher to process the data

  publisher.PublishSessions(sessions);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Wait for the publisher to process the data

  publisher.Shutdown();
}

namespace
{
  // Reads back a single row via the raw sqlite3 API (rather than CSQLiteDriver)
  // specifically so the test can assert on the real SQLite storage class of
  // `expired` - CSQLiteDriver's callback always coerces column values to
  // text, which would hide a regression to TEXT storage.
  struct RawRow
  {
    bool found = false;
    int expiredColumnType = SQLITE_NULL;
    std::string sess;
  };

  RawRow ReadSessionRow(const std::string& dbFilename, const std::string& sid)
  {
    RawRow row;
    sqlite3* db = nullptr;
    if (sqlite3_open(dbFilename.c_str(), &db) != SQLITE_OK)
    {
      return row;
    }

    sqlite3_stmt* stmt = nullptr;
    const std::string sql = "SELECT expired, sess FROM sessions WHERE sid = ?;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
      sqlite3_bind_text(stmt, 1, sid.c_str(), -1, SQLITE_TRANSIENT);
      if (sqlite3_step(stmt) == SQLITE_ROW)
      {
        row.found = true;
        row.expiredColumnType = sqlite3_column_type(stmt, 0);
        const unsigned char* sessText = sqlite3_column_text(stmt, 1);
        row.sess = sessText ? reinterpret_cast<const char*>(sessText) : "";
      }
      sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return row;
  }
}

TEST(CSQLiteSessionPublisherTests, PublishSession_WritesConnectSqlite3CompatibleRow)
{
  const std::string dbFilename = "sessionPublisherTest.db";
  std::filesystem::remove(dbFilename);

  // CSQLiteSessionPublisher reads its own dbPath from config, so give it a
  // real config file to load rather than a hand-built CConfiguration.
  const std::string configFilename = "sessionPublisherTest.conf";
  {
    std::ofstream configFile(configFilename);
    configFile << "sqlite3.SessionExpressPublisher.dbPath=" << dbFilename << "\n";
  }
  CConfiguration config;
  ASSERT_TRUE(config.LoadConfig(configFilename));

  CSQLiteSessionPublisher publisher(config);
  ASSERT_TRUE(publisher.Initialize());

  CSession session("sid-123", "alice", "Alice Doe", "argon2id", "irrelevant-hash");
  session.SetMaxAge(180);
  session.SetSessionExpires(1893456000); // fixed timestamp for a deterministic ISO string
  session.UpdateUserSessionState(UserSessionState::VALID);
  session.AddUserRole("ADMIN");
  session.AddPrivilege("PURGE_TABLE");

  tSessionList sessions{ session };
  publisher.PublishSession(sessions);

  const RawRow row = ReadSessionRow(dbFilename, "sid-123");
  ASSERT_TRUE(row.found);

  // The `expired` column must be a real SQLite INTEGER, not TEXT, or a
  // Node client's numeric "? <= expired" comparison would never match.
  EXPECT_EQ(row.expiredColumnType, SQLITE_INTEGER);

  const auto sessJson = nlohmann::json::parse(row.sess);
  EXPECT_EQ(sessJson.at("userId"), "alice");
  EXPECT_EQ(sessJson.at("userName"), "Alice Doe");
  EXPECT_EQ(sessJson.at("state"), "VALID");
  EXPECT_EQ(sessJson.at("roles"), nlohmann::json::array({"ADMIN"}));
  EXPECT_EQ(sessJson.at("privileges"), nlohmann::json::array({"PURGE_TABLE"}));
  EXPECT_EQ(sessJson.at("cookie").at("originalMaxAge"), 180000);
  EXPECT_FALSE(sessJson.at("cookie").at("expires").get<std::string>().empty());

  // A second publish that no longer includes sid-123 must remove it - the
  // table is meant to always reflect exactly the latest snapshot.
  tSessionList emptySessions;
  publisher.PublishSession(emptySessions);
  const RawRow rowAfter = ReadSessionRow(dbFilename, "sid-123");
  EXPECT_FALSE(rowAfter.found);

  publisher.Shutdown();
  std::filesystem::remove(dbFilename);
  std::filesystem::remove(configFilename);
}

TEST(CSQLiteSessionPublisherTests, Initialize_UnconfiguredIsANoOp)
{
  // No sqlite3.SessionExpressPublisher.dbPath set - the publisher must stay
  // inert rather than fail or create a database file anywhere.
  CConfiguration config;
  CSQLiteSessionPublisher publisher(config);

  EXPECT_TRUE(publisher.Initialize());

  tSessionList sessions{ CSession("sid-999", "bob", "Bob", "sha256", "hash") };
  publisher.PublishSession(sessions); // must not throw/crash despite no DB open

  publisher.Shutdown();
}