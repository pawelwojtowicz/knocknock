#include "CSessionPublisher.h"
#include "CConfiguration.h"
#include <CTokenizer.h>
#include <Logger.h>

namespace knocknock
{
CSessionPublisher::CSessionPublisher()
: m_dataReady(false)
, m_running(false)
, m_publisherThread()
, m_publishers()
{

}

bool CSessionPublisher::Initialize( CConfiguration& config, const CSecurityIndustrialComplex& factories )
{
  const auto publishers = CTokenizer::Tokenize(config.GetParamString(cParamName_SessionPublishers, cParamValue_DefaultSessionPublishers), ',');

  for (const auto& publisherName : publishers)
  {
    auto publisher = factories.GetSessionPublisherFactory().Create(publisherName);
    if ( publisher )
    {
      if (publisher->Initialize(config))
      {
        m_publishers.push_back( std::move(publisher )  );
      }
      else
      {
        LOG(ERROR, "CSessionPublisher - Failed to Initialize [%s] - skipping", publisherName.c_str() );
      }
    }
    else
    {
      LOG(ERROR, "CSessionPublisher - No [%s] - available in the factory", publisherName.c_str() );
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

