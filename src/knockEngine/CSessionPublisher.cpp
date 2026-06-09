#include "CSessionPublisher.h"
#include "CConfiguration.h"
#include <iostream>

namespace knocknock
{
CSessionPublisher::CSessionPublisher()
: m_dataReady(false)
, m_running(false)
, m_publisherThread()
, m_publishers()
{

}

bool CSessionPublisher::Initialize( CConfiguration& config )
{
  for (const auto& publisher : m_publishers)
  {
    if (!publisher->Initialize())
    {
      std::cerr << "Failed to initialize a session publisher." << std::endl;
      return false;
    }
  }

  // Implementation for initialization
  
  m_running = true;
  m_publisherThread = std::thread([this]() { Run(); });
  return true;
}

void CSessionPublisher::Shutdown()
{
  // Implementation for shutdown
  m_running = false;
  m_dataReadyCondition.notify_one();
  if (m_publisherThread.joinable())
  {
    m_publisherThread.join();
  }

  for (const auto& publisher : m_publishers)
  {
    publisher->Shutdown();
  }
}

void CSessionPublisher::PublishSessions( tSessionList& activeSessions)
{
  std::lock_guard<std::mutex> lock(m_dataReadyMutex);
  m_sessionData = std::move(activeSessions);
  m_dataReady = true;
  m_dataReadyCondition.notify_one();
}

void CSessionPublisher::Run()
{
  while (m_running)
  {
    bool hasDataToPublish = false;
    tSessionList dataToPublish;
    {
      std::unique_lock<std::mutex> lock(m_dataReadyMutex);

      m_dataReadyCondition.wait(lock, [this] { return m_dataReady || !m_running; });
      if (m_dataReady)
      {
        hasDataToPublish = true;
        dataToPublish = std::move(m_sessionData);
        m_dataReady = false;
      }
    }

    if (hasDataToPublish)
    {
      for (const auto& publisher : m_publishers)
      {
        publisher->PublishSession(dataToPublish);
      }
    }
  }
  // Implementation for the main loop or processing
};

}

