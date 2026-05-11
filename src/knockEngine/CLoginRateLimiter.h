#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <mutex>

namespace knocknock
{

class CLoginRateLimiter
{
public:
  CLoginRateLimiter();
  ~CLoginRateLimiter() = default;

  void Configure(int maxAttempts, int64_t baseLockoutSeconds);

  // Returns true if the login attempt is allowed, false if rate-limited
  bool IsLoginAllowed(const std::string& userId, int64_t currentTime);

  // Record a failed login attempt for the given userId
  void RecordFailure(const std::string& userId, int64_t currentTime);

  // Reset the failure counter (call on successful login)
  void RecordSuccess(const std::string& userId);

  // Remove expired lockout entries (called periodically)
  void Cleanup(int64_t currentTime);

private:
  struct LoginAttemptRecord
  {
    int failureCount = 0;
    int64_t lockoutExpiry = 0;
  };

  using tAttemptMap = std::map<std::string, LoginAttemptRecord>;

  int m_maxAttempts;
  int64_t m_baseLockoutSeconds;

  std::mutex m_mutex;
  tAttemptMap m_attempts;
};

}
