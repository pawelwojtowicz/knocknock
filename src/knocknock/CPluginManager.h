#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <dlfcn.h>
#include <IKnockKnockPlugin.h>

namespace knocknock
{

class CPluginManager
{
  using tPluginTuple = std::tuple<void*, std::unique_ptr<IKnockKnockPlugin>>;
  using tPluginList = std::vector<tPluginTuple>;
public:
  CPluginManager() = default;
  virtual ~CPluginManager();

  bool Initialize();
  void Shutdown();

private:
  bool LoadPlugin(const std::string& pluginPath);

  tPluginList m_plugins;
};

}