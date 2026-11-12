#include "CMQTTSessionPublisher.h"
#include <iostream>

const std::string DFLT_ADDRESS { "tcp://127.0.0.1:1883" };
const std::string TOPIC { "data/rand" };
const int	 QOS = 1;
const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering
const std::string PERSIST_DIR { "data-persist" };

namespace MQTTConnectivity
{
bool CMQTTSessionPublisher::Initialize( const CConfiguration& rConfig )
{
  std::cout << "initialize MQTT" << std::endl;
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
    std::cerr << exc.what() << std::endl;
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
    std::cout << "shutdown MQTT" << std::endl;

    m_asynClient->disconnect()->wait();
  }
  catch (const mqtt::exception& exc) 
  {
    std::cerr << exc.what() << std::endl;
  }
}
  
void CMQTTSessionPublisher::PublishSession( const tSessionList& activeSessions )
{
  mqtt::topic top(*m_asynClient, TOPIC, QOS, true);
  top.publish("temp");
  std::cout << "publish" << std::endl;
}

}