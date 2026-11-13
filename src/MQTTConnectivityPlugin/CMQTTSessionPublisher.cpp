#include "CMQTTSessionPublisher.h"
#include <nlohmann/json.hpp>
#include <Logger.h>

const std::string DFLT_ADDRESS { "tcp://127.0.0.1:1883" };
const std::string TOPIC { "data/rand" };
const int	 QOS = 1;
const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering
const std::string PERSIST_DIR { "data-persist" };

namespace MQTTConnectivity
{
bool CMQTTSessionPublisher::Initialize( const CConfiguration& rConfig )
{
  LOG(INFO, "initialize MQTT");
  m_asynClient = std::make_unique<mqtt::async_client>(DFLT_ADDRESS, "", MAX_BUFFERED_MSGS);

  mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * 5);
	connOpts.set_clean_session(true);
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

  tSessionList activeSessions;
  PublishSession(activeSessions );

  return true;
}

void CMQTTSessionPublisher::Shutdown()
{
  try
  {
    LOG(INFO, "shutdown MQTT");

    m_asynClient->disconnect()->wait();
  }
  catch (const mqtt::exception& exc) 
  {
    LOG(ERROR, "CMQTTSessionPublisher: Shutdown error: %s", exc.what());
  }
}
  
void CMQTTSessionPublisher::PublishSession( const tSessionList& activeSessions )
{
  mqtt::topic top(*m_asynClient, TOPIC, QOS, true);
  try 
  {
    top.publish(BuildSessionJSON(activeSessions));
  }
  catch (const mqtt::exception& exc)
  {
    LOG(ERROR, "CMQTTSessionPublisher::PublishSession error: %s", exc.what());
  }
}

const char* ToString( UserSessionState state )
{
  switch (state)
  {
    case UserSessionState::INVALID_SESSION:  return "INVALID_SESSION";
    case UserSessionState::CREATED:          return "CREATED";
    case UserSessionState::AUTH_IN_PROGRESS: return "AUTH_IN_PROGRESS";
    case UserSessionState::AUTH_SUCCESS:     return "AUTH_SUCCESS";
    case UserSessionState::AUTH_FAILED:      return "AUTH_FAILED";
    case UserSessionState::VALID:            return "VALID";
    case UserSessionState::EXPIRED:          return "EXPIRED";
    case UserSessionState::LOGGED_OUT:       return "LOGGED_OUT";
  }
  return "UNKNOWN";
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
    jsonSession["state"] = ToString(session.GetUserSessionState());
    jsonSession["roles"] = session.GetRoles();
    jsonSession["privileges"] = session.GetPrivileges();
    jsonSessions.push_back(jsonSession);
  }

  return jsonSessions.dump();
}

}