#pragma once
#include <ISessionPublisher.h>
#include <CConfiguration.h>
#include <CSession.h>
#include <memory>
#include <mqtt/async_client.h>

using namespace knocknock;

namespace MQTTConnectivity
{
class CMQTTSessionPublisher : public knocknock::ISessionPublisher
{
public:
  CMQTTSessionPublisher() = default;
  virtual ~CMQTTSessionPublisher() = default;

  virtual bool Initialize( const CConfiguration& rConfig ) override;
  virtual void Shutdown() override;
  
  virtual void PublishSession( const tSessionList& activeSessions ) override;

private:
  static std::string BuildSessionJSON( const tSessionList& activeSessions );

private:
  std::unique_ptr<mqtt::async_client> m_asynClient;

};
}