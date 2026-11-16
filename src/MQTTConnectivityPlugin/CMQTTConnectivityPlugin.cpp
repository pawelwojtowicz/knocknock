#include "CMQTTConnectivityPlugin.h"
#include "CMQTTSessionPublisher.h"

namespace MQTTConnectivity
{
CMQTTConnectivityPlugin::CMQTTConnectivityPlugin()
  : knocknock::CPluginBase( "MQTTConnectivityPlugin", "1.0.0" )
{
  RegisterSessionPublisher("mqtt", []() { return std::make_unique<CMQTTSessionPublisher>(); });
}

}
