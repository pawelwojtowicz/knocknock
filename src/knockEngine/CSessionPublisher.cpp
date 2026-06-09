#include "CSessionPublisher.h"
#include "CConfiguration.h"
#include <iostream>

namespace knocknock
{
CSessionPublisher::CSessionPublisher()
: m_dataReady(false)
, m_sessionData("")
, m_running(false)
, m_publisherThread()
{

}

bool CSessionPublisher::Initialize( CConfiguration& config )
{
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
}

void CSessionPublisher::PublishSessions( const std::string& data)
{
  // Implementation for publishing sessions
  std::cout << "PublishSessions: before mutex" << std::endl;
  std::lock_guard<std::mutex> lock(m_dataReadyMutex);
  std::cout << "PublishSessions: after mutex" << std::endl;

  m_sessionData = data;
  m_dataReady = true;
  m_dataReadyCondition.notify_one();
}

void CSessionPublisher::Run()
{
  while (m_running)
  {
    std::cout << "Run: before mutex" << std::endl;

    std::string dataToPublish{};
    {
      std::unique_lock<std::mutex> lock(m_dataReadyMutex);
      std::cout << "Run: beforeAfter mutex" << std::endl;

      m_dataReadyCondition.wait(lock, [this] { return m_dataReady || !m_running; });
      std::cout << "Run: after mutex" << std::endl;
      if (m_dataReady)
      {
        std::cout << "Run: data ready" << std::endl;
        dataToPublish = m_sessionData;
        m_dataReady = false;
      }
    }

    if (!dataToPublish.empty())
    {
      for (int i = 0 ; i < 5; ++i) // Simulate publishing data multiple times
      {
        // Simulate publishing the session data
        std::cout << "Publishing session data: " << dataToPublish << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate time taken to publish
      }
    }
  }
  // Implementation for the main loop or processing
};

}

