#include "CMQTTConnectivityPlugin.h"
#include <memory>

extern "C"
{

std::unique_ptr<knocknock::IKnockKnockPlugin> createInstance()
{
  return std::make_unique<CMQTTConnectivityPlugin::CMQTTConnectivityPlugin>();
}

}