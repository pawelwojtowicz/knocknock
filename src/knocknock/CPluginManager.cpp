#include "CPluginManager.h"
#include <filesystem>
#include <memory>
#include <variant>
#include <dlfcn.h>
#include <iostream>
#include <Logger.h>

namespace knocknock
{

CPluginManager::~CPluginManager()
{
  for (auto& pluginTuple : m_plugins)
  {
    void* fileHandle = std::get<0>(pluginTuple);
    if (fileHandle)
    {
      dlclose(fileHandle);
    }
  }
}

using tPrCreatePluginInstanceFunc = std::unique_ptr<knocknock::IKnockKnockPlugin> (*)();

bool CPluginManager::Initialize()
{
  const std::string pluginPath = "/home/pawel/knocknock/build/src/MQTTConnectivityPlugin/libmqttConnectivityPlugin.so";
  return LoadPlugin(pluginPath);
}

void CPluginManager::Shutdown()
{

}

bool CPluginManager::LoadPlugin(const std::string& pluginPath)
{
  // if file exists
  if ( std::filesystem::exists( pluginPath ) )
  {
    LOG( INFO, "Loading plugin from path: %s", pluginPath.c_str() );
    void* fileHandle = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if (fileHandle != nullptr)
    {
      LOG( INFO, "Plugin loaded successfully: %s", pluginPath.c_str() );
      tPrCreatePluginInstanceFunc componentGetter = reinterpret_cast<tPrCreatePluginInstanceFunc> (dlsym( fileHandle, "createInstance") );
      if ( componentGetter != nullptr )
      {
        LOG( INFO, "Found createInstance function in plugin: %s", pluginPath.c_str() );
        std::unique_ptr<knocknock::IKnockKnockPlugin> pluginInstance = componentGetter();
        if (pluginInstance)
        {
          LOG( INFO, "Loaded plugin: %s Version: %s", pluginInstance->GetName().c_str(), pluginInstance->GetVersion().c_str() );
          m_plugins.emplace_back(fileHandle, std::move(pluginInstance));
          // Successfully created plugin instance
          return true;
        }
      }
    }
    else
    {
      LOG( ERROR, "Failed to load plugin: %s", pluginPath.c_str() );
    }
  }
  else
  {
    LOG( ERROR, "Plugin file not found: %s", pluginPath.c_str() );
  }
  return false;
}

}