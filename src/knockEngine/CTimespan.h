#pragma once

namespace knocknock
{
class CTimespan
{
private:
  CTimespan() = default;

public:
  static int GetEpochSeconds();
  static void SetTimeline(int seconds);
  static void AddTimespan(int seconds);

private:
  static int m_timestamp;
};
} 