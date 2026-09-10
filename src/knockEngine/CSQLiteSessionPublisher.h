#pragma once
#include <string>
#include <CSQLiteDriver.h>
#include "ISessionPublisher.h"

namespace knocknock
{
class CConfiguration;

// Publishes session snapshots into a SQLite table compatible with the
// connect-sqlite3 store for express-session: a 3-column, untyped table
// (sid PRIMARY KEY, expired, sess) where `sess` is the JSON-serialized
// session object and `expired` is a unix-epoch-milliseconds integer. A
// Node/Express app using express-session + connect-sqlite3 pointed at the
// same file can read sessions created by knocknock directly.
//
// Uses its own dedicated SQLite connection/file, separate from the rest of
// the application's database, so this publisher's writes (from the
// CSessionPublisher background thread) never contend with request-handling
// reads on the main DB connection.
class CSQLiteSessionPublisher : public ISessionPublisher
{
public:
  // Takes the shared configuration by reference (stored, not copied) rather
  // than an already-resolved path, so this class - not its caller - owns
  // reading and interpreting its own config keys (e.g. whether it's enabled
  // at all). Mirrors CSessionBuilder's constructor-injected-config pattern.
  explicit CSQLiteSessionPublisher( const CConfiguration& config, const std::string& tableName = "sessions" );
  ~CSQLiteSessionPublisher() override = default;

  bool Initialize() override;
  void Shutdown() override;

  void PublishSession( const tSessionList& activeSessions ) override;

private:
  static std::string BuildSessionJSON( const CSession& session );

private:
  const CConfiguration& m_rConfiguration;

  // Resolved from config in Initialize(). Empty means "not configured" -
  // this publisher then stays a no-op for its whole lifetime.
  std::string m_dbFilename;
  std::string m_tableName;
  DBAccess::CSQLiteDriver m_dbDriver;
};

}
