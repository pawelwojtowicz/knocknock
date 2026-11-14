#pragma once
#include <ISessionPublisher.h>
#include <CConfiguration.h>
#include <CSession.h>
#include <memory>
#include <mqtt/async_client.h>

using namespace knocknock;

namespace MQTTConnectivity
{
class CMQTTSessionPublisher : public knocknock::ISessionPublisher, public mqtt::callback
{
public:
  CMQTTSessionPublisher() = default;
  virtual ~CMQTTSessionPublisher() = default;

  virtual bool Initialize( const CConfiguration& rConfig ) override;
  virtual void Shutdown() override;
  
  virtual void PublishSession( const tSessionList& activeSessions ) override;

private:
  void connected(const std::string& cause) override;

private:
  static std::string BuildSessionJSON( const tSessionList& activeSessions );

private:
  std::unique_ptr<mqtt::async_client> m_asynClient{};
  std::unique_ptr<mqtt::topic> m_topicHelper{};

  std::string m_brokerAddress{"tcp://127.0.0.1:1883"};
  std::string m_clientId{"knocknock"};
  std::string m_userName{""};
  std::string m_password{""};
  std::string m_topic{"userSessions"};

  std::string m_lastPayload{"[]"};

};
}