#pragma once
#include <cstdint>

namespace Utilities
{
class CTimespan
{
private:
  CTimespan() = default;

public:
  static int64_t GetEpochSeconds();
  static void SetTimeline(int64_t seconds);
  static void AddTimespan(int64_t seconds);

private:
  static int64_t m_timestamp;
};
} 