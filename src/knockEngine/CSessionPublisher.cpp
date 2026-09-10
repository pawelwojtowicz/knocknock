#include "CSessionPublisher.h"
#include "CConfiguration.h"
#include "CSQLiteSessionPublisher.h"
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
  // Build the publisher list from available plugins. This is the only place
  // m_publishers is ever populated - it is fixed for the rest of this
  // object's lifetime once Initialize() returns. Each publisher owns
  // reading and interpreting its own config keys (including whether it's
  // enabled at all) - CSessionPublisher just wires the shared config in.
  m_publishers.push_back(std::make_shared<CSQLiteSessionPublisher>(config));

  for (const auto& publisher : m_publishers)
  {
    if (!publisher->Initialize())
    {
      std::cerr << "Failed to initialize a session publisher." << std::endl;
      return false;
    }
  }

  m_running = true;
  m_publisherThread = std::thread([this]() { Run(); });
  return true;
}

void CSessionPublisher::Shutdown()
{
  // Signal Run() to stop and join it. Any snapshot sitting in m_sessionData
  // at this point (from a PublishSessions() call the background thread
  // hasn't drained yet) is intentionally discarded rather than published.
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
  // Latest-wins by design: if a previous snapshot hasn't been picked up by
  // Run() yet, it is overwritten and never published. Callers that need
  // every snapshot delivered should not rely on this method.
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
};

}

