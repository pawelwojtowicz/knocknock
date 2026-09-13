#pragma once
#include <CPluginBase.h>

namespace CMQTTConnectivityPlugin
{

class CMQTTConnectivityPlugin :public knocknock::CPluginBase
{
public:
  CMQTTConnectivityPlugin();
  virtual ~CMQTTConnectivityPlugin() = default;
};
}
