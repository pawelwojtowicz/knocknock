#include "CLoginRateLimiter.h"
#include "CConfiguration.h"
#include "KnocKnockDictionary.h"
#include <Logger.h>
#include <algorithm>

namespace knocknock
{

CLoginRateLimiter::CLoginRateLimiter()
: m_maxAttempts{cParamValue_MaxLoginAttempts}
, m_baseLockoutSeconds{cParamValue_LoginLockoutSeconds}
, m_failureResetSeconds{cParamValue_LoginFailureResetSeconds}
, m_attemptsMutex()
, m_attempts()
{
}

bool CLoginRateLimiter::Initialize( const CConfiguration& rConfig )
{
  m_maxAttempts = rConfig.GetParamInt(cParamName_MaxLoginAttempts, cParamValue_MaxLoginAttempts);
  m_baseLockoutSeconds = rConfig.GetParamInt(cParamName_LoginLockoutSeconds, cParamValue_LoginLockoutSeconds);
  m_failureResetSeconds = rConfig.GetParamInt(cParamName_LoginFailureResetSeconds, cParamValue_LoginFailureResetSeconds);

  if ( m_maxAttempts < 1 || m_baseLockoutSeconds < 0 || m_failureResetSeconds < 0 )
  {
    LOG(ERROR, "CLoginRateLimiter - invalid configuration: %s=%d, %s=%lld, %s=%lld",
        cParamName_MaxLoginAttempts, m_maxAttempts,
        cParamName_LoginLockoutSeconds, static_cast<long long>(m_baseLockoutSeconds),
        cParamName_LoginFailureResetSeconds, static_cast<long long>(m_failureResetSeconds) );
    return false;
  }

  return true;
}

bool CLoginRateLimiter::IsStale( const LoginAttemptRecord& record, int64_t currentTime ) const
{
  // Dropping a record as soon as its lockout expires would reset
  // failureCount and defeat the exponential backoff.
  return record.lockoutExpiry <= currentTime &&
         currentTime - record.lastFailureTime >= m_failureResetSeconds;
}

bool CLoginRateLimiter::IsLoginAllowed( const std::string& userId, int64_t currentTime )
{
  std::lock_guard<std::mutex> lock(m_attemptsMutex);
  auto it = m_attempts.find(userId);
  if (it == m_attempts.end())
  {
    return true;
  }

  // If lockout has expired, allow the attempt
  return it->second.lockoutExpiry <= currentTime;
}

void CLoginRateLimiter::RecordFailure( const std::string& userId, int64_t currentTime )
{
  std::lock_guard<std::mutex> lock(m_attemptsMutex);
  auto it = m_attempts.try_emplace(userId).first;
  LoginAttemptRecord& record = it->second;
  if (record.failureCount > 0 && IsStale(record, currentTime))
  {
    // Old history that Cleanup() hasn't purged yet - start fresh
    record = LoginAttemptRecord{};
  }
  record.failureCount++;
  record.lastFailureTime = currentTime;

  if (record.failureCount >= m_maxAttempts)
  {
    // Exponential backoff: baseLockout * 2^(excess failures)
    int excessFailures = record.failureCount - m_maxAttempts;
    int64_t lockoutDuration = m_baseLockoutSeconds * (1 << std::min(excessFailures, 6)); // cap at 64x
    record.lockoutExpiry = currentTime + lockoutDuration;
  }
}

void CLoginRateLimiter::RecordSuccess( const std::string& userId )
{
  std::lock_guard<std::mutex> lock(m_attemptsMutex);
  m_attempts.erase(userId);
}

void CLoginRateLimiter::Cleanup( int64_t currentTime )
{
  std::lock_guard<std::mutex> lock(m_attemptsMutex);
  for (auto it = m_attempts.begin(); it != m_attempts.end(); )
  {
    if (IsStale(it->second, currentTime))
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
