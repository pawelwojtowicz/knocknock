#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <mutex>

namespace knocknock
{
class CConfiguration;

class CLoginRateLimiter
{
public:
  CLoginRateLimiter();
  ~CLoginRateLimiter() = default;

  // Reads maxLoginAttempts, loginLockoutSeconds and loginFailureResetSeconds
  // from the configuration. Returns false if any of them is out of range.
  bool Initialize( const CConfiguration& rConfig );

  // Returns true if the login attempt is allowed, false if rate-limited
  bool IsLoginAllowed( const std::string& userId, int64_t currentTime );

  // Record a failed login attempt for the given userId
  void RecordFailure( const std::string& userId, int64_t currentTime );

  // Reset the failure counter (call on successful login)
  void RecordSuccess( const std::string& userId );

  // Remove entries with no failure for m_failureResetSeconds and no active
  // lockout (called periodically)
  void Cleanup( int64_t currentTime );

private:
  struct LoginAttemptRecord
  {
    int failureCount = { 0 };
    int64_t lockoutExpiry = { 0 };
    int64_t lastFailureTime = { 0 };
  };

  using tAttemptMap = std::map<std::string, LoginAttemptRecord>;

private:
  // A record is stale once any lockout is over AND the user has gone
  // m_failureResetSeconds without another failure
  bool IsStale( const LoginAttemptRecord& record, int64_t currentTime ) const;

private:
  // number of failures allowed before the first lockout
  int m_maxAttempts;

  // lockout length after reaching m_maxAttempts; doubles with every further failure (capped at 64x)
  int64_t m_baseLockoutSeconds;

  // how long without a failure before a user's failure history is forgotten
  int64_t m_failureResetSeconds;

  std::mutex m_attemptsMutex;

  tAttemptMap m_attempts;
};

}
