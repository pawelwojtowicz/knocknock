#pragma once
#include <CPluginBase.h>

namespace MQTTConnectivity
{

class CMQTTConnectivityPlugin :public knocknock::CPluginBase
{
public:
  CMQTTConnectivityPlugin();
  virtual ~CMQTTConnectivityPlugin() = default;
};
}
