#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include "CSession.h"

namespace knocknock
{
class CConfiguration;

class CSessionPublisher
{
public:
  CSessionPublisher();
  virtual ~CSessionPublisher() = default;

  bool Initialize( CConfiguration& config );
  void Shutdown();

  void PublishSessions( const std::string& data);

private:
  void Run();

private:
  bool m_dataReady;
  std::string m_sessionData;
  std::atomic<bool> m_running;
  std::thread m_publisherThread;

  std::mutex m_dataReadyMutex;
  std::condition_variable m_dataReadyCondition;

}; 
}