#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include "CSession.h"
#include "ISessionPublisher.h"

namespace knocknock
{
class CConfiguration;

class CSessionPublisher
{
  using tSessionPublishers = std::vector<std::shared_ptr<ISessionPublisher>>;
public:
  CSessionPublisher();
  virtual ~CSessionPublisher() = default;

  bool Initialize( CConfiguration& config );
  void Shutdown();

  void PublishSessions( tSessionList& activeSessions );

private:
  void Run();

private:
  bool m_dataReady;
  tSessionList m_sessionData;
  std::atomic<bool> m_running;
  std::thread m_publisherThread;

  std::mutex m_dataReadyMutex;
  std::condition_variable m_dataReadyCondition;

  tSessionPublishers m_publishers;
}; 
}