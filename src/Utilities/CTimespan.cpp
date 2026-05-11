#include "CTimespan.h"
#include <chrono>

namespace Utilities
{
int64_t CTimespan::m_timestamp = -1;

int64_t CTimespan::GetEpochSeconds()
{
  if (m_timestamp < 0 )
  {
    // get the current time
    const auto now     = std::chrono::system_clock::now();
    // transform the time into a duration since the epoch
    const auto epoch   = now.time_since_epoch();
    // cast the duration into seconds
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    
    // return the number of seconds
    return seconds.count();
  }
  return m_timestamp;
}

void CTimespan::SetTimeline(int64_t seconds)
{
  m_timestamp = seconds;
}

void CTimespan::AddTimespan(int64_t seconds)
{
  m_timestamp += seconds;
}

}