#pragma once
#include <IKnockKnockPlugin.h>

namespace CMQTTConnectivityPlugin
{

class CMQTTConnectivityPlugin :public knocknock::IKnockKnockPlugin
{
public:
  CMQTTConnectivityPlugin() = default;
  virtual ~CMQTTConnectivityPlugin() = default;

  std::string GetName() const override;
  std::string GetVersion() const override;
};
}