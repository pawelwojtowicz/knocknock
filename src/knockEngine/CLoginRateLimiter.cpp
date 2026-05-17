#include "CLoginRateLimiter.h"
#include <algorithm>

namespace knocknock
{

CLoginRateLimiter::CLoginRateLimiter()
: m_maxAttempts{5}
, m_baseLockoutSeconds{30}
, m_mutex()
, m_attempts()
{
}

void CLoginRateLimiter::Configure(int maxAttempts, int64_t baseLockoutSeconds)
{
  m_maxAttempts = maxAttempts;
  m_baseLockoutSeconds = baseLockoutSeconds;
}

bool CLoginRateLimiter::IsLoginAllowed(const std::string& userId, int64_t currentTime)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  auto it = m_attempts.find(userId);
  if (it == m_attempts.end())
  {
    return true;
  }

  const LoginAttemptRecord& record = it->second;

  // If lockout has expired, allow the attempt
  if (record.lockoutExpiry <= currentTime)
  {
    return true;
  }

  return false;
}

void CLoginRateLimiter::RecordFailure(const std::string& userId, int64_t currentTime)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  auto it = m_attempts.try_emplace(userId).first;
  LoginAttemptRecord& record = it->second;
  record.failureCount++;

  if (record.failureCount >= m_maxAttempts)
  {
    // Exponential backoff: baseLockout * 2^(excess failures)
    int excessFailures = record.failureCount - m_maxAttempts;
    int64_t lockoutDuration = m_baseLockoutSeconds * (1 << std::min(excessFailures, 6)); // cap at 64x
    record.lockoutExpiry = currentTime + lockoutDuration;
  }
}

void CLoginRateLimiter::RecordSuccess(const std::string& userId)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_attempts.erase(userId);
}

void CLoginRateLimiter::Cleanup(int64_t currentTime)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  for (auto it = m_attempts.begin(); it != m_attempts.end(); )
  {
    // Remove entries whose lockout has expired
    if (it->second.lockoutExpiry > 0 && it->second.lockoutExpiry <= currentTime)
    {
      it = m_attempts.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

}
