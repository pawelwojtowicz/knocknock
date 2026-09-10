#include "CSQLiteSessionPublisher.h"
#include "CConfiguration.h"
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>

namespace knocknock
{
namespace
{

// Path to this publisher's dedicated SQLite file. Left empty (the default)
// disables it - this is the only config key CSQLiteSessionPublisher needs,
// and CSessionPublisher has no knowledge of it.
constexpr auto cParamName_SQLiteDbPath = "sqlite3.SessionExpressPublisher.dbPath";
constexpr auto cParamValue_SQLiteDbPath = "";

const char* ToString( UserSessionState state )
{
  switch (state)
  {
    case UserSessionState::INVALID_SESSION:  return "INVALID_SESSION";
    case UserSessionState::CREATED:          return "CREATED";
    case UserSessionState::AUTH_IN_PROGRESS: return "AUTH_IN_PROGRESS";
    case UserSessionState::AUTH_SUCCESS:     return "AUTH_SUCCESS";
    case UserSessionState::AUTH_FAILED:      return "AUTH_FAILED";
    case UserSessionState::VALID:            return "VALID";
    case UserSessionState::EXPIRED:          return "EXPIRED";
    case UserSessionState::LOGGED_OUT:       return "LOGGED_OUT";
  }
  return "UNKNOWN";
}

// Formats an epoch-seconds timestamp the way JS's Date.toISOString() would,
// matching what express-session itself writes into cookie.expires.
std::string ToISO8601( int64_t epochSeconds )
{
  std::time_t time = static_cast<std::time_t>(epochSeconds);
  std::tm utcTime{};
  gmtime_r(&time, &utcTime);

  char buffer[32];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &utcTime);
  return std::string(buffer) + ".000Z";
}

}

CSQLiteSessionPublisher::CSQLiteSessionPublisher( const CConfiguration& config, const std::string& tableName )
: m_rConfiguration(config)
, m_dbFilename()
, m_tableName(tableName)
, m_dbDriver()
{
}

bool CSQLiteSessionPublisher::Initialize()
{
  m_dbFilename = m_rConfiguration.GetParamString(cParamName_SQLiteDbPath, cParamValue_SQLiteDbPath);
  if (m_dbFilename.empty())
  {
    // Not configured - stay disabled. PublishSession()/Shutdown() no-op.
    return true;
  }

  if (!m_dbDriver.Open(m_dbFilename))
  {
    std::cerr << "CSQLiteSessionPublisher: failed to open " << m_dbFilename << std::endl;
    return false;
  }

  // WAL mode + a busy timeout let an external reader (e.g. a Node process
  // using connect-sqlite3 against this same file) read sessions without
  // being blocked by this publisher's writes, and vice versa.
  m_dbDriver.ExecuteSQLCommand("PRAGMA journal_mode = WAL;", nullptr, nullptr);
  m_dbDriver.ExecuteSQLCommand("PRAGMA busy_timeout = 5000;", nullptr, nullptr);

  // Schema matches connect-sqlite3's own CREATE TABLE exactly (untyped
  // columns) so an express-session app pointed at this file needs no
  // migration of its own.
  const std::string createTableSql = "CREATE TABLE IF NOT EXISTS " + m_tableName + " (sid PRIMARY KEY, expired, sess);";
  if (!m_dbDriver.ExecuteSQLCommand(createTableSql, nullptr, nullptr))
  {
    std::cerr << "CSQLiteSessionPublisher: failed to create table " << m_tableName << std::endl;
    return false;
  }

  return true;
}

void CSQLiteSessionPublisher::Shutdown()
{
  if (m_dbFilename.empty())
  {
    return;
  }
  m_dbDriver.Close();
}

void CSQLiteSessionPublisher::PublishSession( const tSessionList& activeSessions )
{
  if (m_dbFilename.empty())
  {
    return;
  }

  if (!m_dbDriver.ExecuteSQLCommand("BEGIN TRANSACTION;", nullptr, nullptr))
  {
    return;
  }

  if (activeSessions.empty())
  {
    // Nothing active - the table should end up empty too.
    m_dbDriver.ExecuteSQLCommand("DELETE FROM " + m_tableName + ";", nullptr, nullptr);
  }
  else
  {
    // Drop rows for sessions that are no longer active (logged out/expired/
    // purged from memory since the last publish), then upsert the rest, so
    // the table always ends up matching this snapshot exactly.
    std::string deleteStaleSql = "DELETE FROM " + m_tableName + " WHERE sid NOT IN (";
    std::vector<std::string> sids;
    sids.reserve(activeSessions.size());
    for (size_t i = 0; i < activeSessions.size(); ++i)
    {
      deleteStaleSql += (i == 0 ? "?" : ",?");
      sids.push_back(activeSessions[i].GetSessionId());
    }
    deleteStaleSql += ");";
    m_dbDriver.ExecutePreparedStatement(deleteStaleSql, sids, nullptr, nullptr);

    for (const auto& session : activeSessions)
    {
      // `expired` must be a genuine SQLite INTEGER (not TEXT) so a Node
      // client's numeric "? <= expired" comparison (what connect-sqlite3's
      // own get() does) works correctly - hence the literal here rather
      // than a bound parameter (ExecutePreparedStatement only binds TEXT).
      // The value is an int64_t computed by us, never external input, so
      // there is no injection risk in splicing it into the SQL text.
      const int64_t expiredMs = session.GetSessionExpires() * 1000;
      const std::string upsertSql = "INSERT OR REPLACE INTO " + m_tableName +
        " (sid, expired, sess) VALUES (?, " + std::to_string(expiredMs) + ", ?);";
      const std::vector<std::string> params = { session.GetSessionId(), BuildSessionJSON(session) };
      m_dbDriver.ExecutePreparedStatement(upsertSql, params, nullptr, nullptr);
    }
  }

  m_dbDriver.ExecuteSQLCommand("COMMIT;", nullptr, nullptr);
}

std::string CSQLiteSessionPublisher::BuildSessionJSON( const CSession& session )
{
  // Shape mirrors what express-session itself would store: known fields
  // (userId/userName/state/roles/privileges) directly on the object, plus a
  // `cookie` sub-object so express-session's own Cookie reconstruction on
  // load has something sensible to work with. Deliberately excludes
  // authMethod/authString (the credential/hash) and the transient
  // in-progress-challenge state - those don't belong duplicated here.
  nlohmann::json sess;

  sess["userId"] = session.GetUserId();
  sess["userName"] = session.GetUserName();
  sess["state"] = ToString(session.GetUserSessionState());
  sess["roles"] = session.GetRoles();
  sess["privileges"] = session.GetPrivileges();

  sess["cookie"] = {
    { "originalMaxAge", session.GetMaxAge() * 1000 },
    { "expires", ToISO8601(session.GetSessionExpires()) },
    { "httpOnly", true },
    { "path", "/" }
  };

  return sess.dump();
}

}
