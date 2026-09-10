#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include "CSession.h"
#include "ISessionPublisher.h"

namespace knocknock
{
class CConfiguration;

// Fans out session-list snapshots to whatever ISessionPublisher backends are
// configured, off the caller's thread. Delivery is best-effort, not reliable
// queuing: PublishSessions() only ever keeps the most recent snapshot, so if
// it is called again before the background thread has drained the previous
// one, the intermediate snapshot is intentionally dropped. Shutdown() does
// not flush a pending snapshot either - on process exit it is fine to lose
// the last batch rather than delay shutdown waiting for it to publish.
class CSessionPublisher
{
  using tSessionPublishers = std::vector<std::shared_ptr<ISessionPublisher>>;
public:
  CSessionPublisher();
  virtual ~CSessionPublisher() = default;

  // Builds m_publishers from config/available plugins and starts the
  // background thread. Must be called exactly once (never concurrently with
  // itself, and never called again before a matching Shutdown()) - the
  // background-thread lifecycle here doesn't guard against re-entry.
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

  // Populated once in Initialize(), before the background thread is
  // started, and never modified afterward - safe to read from Run() without
  // a lock, since std::thread's constructor happens-before the new thread's
  // first instruction.
  tSessionPublishers m_publishers;
};
}