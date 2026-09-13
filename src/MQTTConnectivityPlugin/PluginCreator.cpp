#include "CMQTTConnectivityPlugin.h"
#include <memory>

extern "C"
{

std::unique_ptr<knocknock::IKnocknockPlugin> createInstance()
{
  return std::make_unique<CMQTTConnectivityPlugin::CMQTTConnectivityPlugin>();
}

}