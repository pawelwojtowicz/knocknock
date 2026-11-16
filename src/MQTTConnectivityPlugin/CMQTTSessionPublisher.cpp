#include "CMQTTSessionPublisher.h"
#include <nlohmann/json.hpp>
#include <Logger.h>
#include <CToStringTools.h>

const int	 QOS = 1;
const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering

namespace MQTTConnectivity
{

constexpr auto s_paramName_BrokerAddress= "sessionPublisher.mqtt.brokerAddress";
constexpr auto s_paramName_ClientId="sessionPublisher.mqtt.clientId";
constexpr auto s_paramName_UserName="sessionPublisher.mqtt.userName";
constexpr auto s_paramName_Password="sessionPublisher.mqtt.password";
constexpr auto s_paramName_Topic="sessionPublisher.mqtt.topic";


bool CMQTTSessionPublisher::Initialize( const CConfiguration& rConfig )
{
  LOG(INFO, "initialize MQTT");
  m_brokerAddress = rConfig.GetParamString(s_paramName_BrokerAddress, m_brokerAddress);
  m_clientId  = rConfig.GetParamString(s_paramName_ClientId,m_clientId);
  m_userName  = rConfig.GetParamString(s_paramName_UserName,m_userName);
  m_password  = rConfig.GetParamString(s_paramName_Password,m_password);
  m_topic = rConfig.GetParamString(s_paramName_Topic,m_topic);

  m_asynClient = std::make_unique<mqtt::async_client>(m_brokerAddress, m_clientId , MAX_BUFFERED_MSGS);
  m_asynClient->set_callback(*this);

  m_topicHelper = std::make_unique<mqtt::topic>(*m_asynClient, m_topic, QOS, true);

  mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * 5);
	connOpts.set_clean_session(true);
  
  if (!m_userName.empty() )
  {
    connOpts.set_user_name(m_userName);
  }
  else
  {
    connOpts.set_user_name(m_clientId);
  }

  if (!m_password.empty())
  {
    connOpts.set_password(m_password);
  }
	connOpts.set_automatic_reconnect(true);

  try
  {
    m_asynClient->connect(connOpts)->wait();
  }
  catch (const mqtt::exception& exc) 
  {
    LOG(ERROR, "CMQTTSessionPublisher: Connect error: %s", exc.what());
    return false;
  }

  return true;
}

void CMQTTSessionPublisher::Shutdown()
{
  LOG(INFO, "Shutdown MQTT");
  try
  {
    m_asynClient->disconnect()->wait();
  }
  catch (const mqtt::exception& exc) 
  {
    LOG(ERROR, "CMQTTSessionPublisher: Shutdown error: %s", exc.what());
  }
}
  
void CMQTTSessionPublisher::PublishSession( const tSessionList& activeSessions )
{
  std::string payload = BuildSessionJSON(activeSessions);

  // Called on the CSessionPublisher thread, while connected() runs on Paho's
  // callback thread - both touch m_lastPayload. The lock also spans the
  // (non-blocking) publish, so a reconnect can't re-publish an older payload
  // after this newer one and leave a stale retained message on the topic.
  std::lock_guard<std::mutex> lock(m_lastPayloadMutex);
  m_lastPayload = std::move(payload);
  PublishLastPayload();
}

void CMQTTSessionPublisher::PublishLastPayload()
{
  try
  {
    m_topicHelper->publish(m_lastPayload);
  }
  catch (const mqtt::exception& exc)
  {
    LOG(ERROR, "CMQTTSessionPublisher::PublishLastPayload error: %s", exc.what());
  }
}

std::string CMQTTSessionPublisher::BuildSessionJSON( const tSessionList& activeSessions )
{
  nlohmann::json jsonSessions = nlohmann::json::array();

  for ( const auto& session : activeSessions)
  {
    nlohmann::json jsonSession;
    jsonSession["sessionId"] = session.GetSessionId();
    jsonSession["userId"] = session.GetUserId();
    jsonSession["userName"] = session.GetUserName();
    jsonSession["state"] = CToStringTools::SessionState(session.GetUserSessionState());
    jsonSession["roles"] = session.GetRoles();
    jsonSession["privileges"] = session.GetPrivileges();
    jsonSessions.push_back(jsonSession);
  }

  return jsonSessions.dump();
}

void CMQTTSessionPublisher::connected(const std::string& cause)
{
  // (Re)connected to the broker - re-publish the latest snapshot
  std::lock_guard<std::mutex> lock(m_lastPayloadMutex);
  PublishLastPayload();
}

}