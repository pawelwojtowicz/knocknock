#pragma once
#include <IKnocknockPlugin.h>

namespace CMQTTConnectivityPlugin
{

class CMQTTConnectivityPlugin :public knocknock::IKnocknockPlugin
{
public:
  CMQTTConnectivityPlugin() = default;
  virtual ~CMQTTConnectivityPlugin() = default;

  std::string GetName() const override;
  std::string GetVersion() const override;
};
}
