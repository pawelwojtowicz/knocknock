#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <dlfcn.h>
#include "IPluginProvider.h"
#include <IKnocknockPlugin.h>

namespace knocknock
{

class CPluginManager : public IPluginProvider
{
public:
  CPluginManager() = default;
  virtual ~CPluginManager();

  bool Initialize();
  void Shutdown();

private:
  const tPluginList& GetPlugins() const { return m_plugins; };

private:
  bool LoadPlugin(const std::string& pluginPath);

  tPluginList m_plugins;
};

}