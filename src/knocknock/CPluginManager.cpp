#include "CPluginManager.h"
#include <CConfiguration.h>
#include <CTokenizer.h>
#include <filesystem>
#include <memory>
#include <variant>
#include <dlfcn.h>
#include <iostream>
#include <Logger.h>

namespace knocknock
{

constexpr auto sParamName_pluginPath = "pluginManager.pluginPath";
constexpr auto sParamName_activePlugins= "pluginManager.activePlugins";

CPluginManager::~CPluginManager()
{
  for (auto& pluginTuple : m_plugins)
  {
    std::get<1>(pluginTuple).reset();
    void* fileHandle = std::get<0>(pluginTuple);
    if (fileHandle)
    {
      dlclose(fileHandle);
    }
  }
}

using tPrCreatePluginInstanceFunc = std::unique_ptr<knocknock::IKnocknockPlugin> (*)();

void CPluginManager::Initialize( CConfiguration& rConfiguration)
{
  const std::string pluginPathStr = rConfiguration.GetParamString( sParamName_pluginPath, "");
  if ( !pluginPathStr.empty() )
  {
    const auto activePlugins = CTokenizer::Tokenize(rConfiguration.GetParamString( sParamName_activePlugins, ""), ',');
    for ( const auto& plugin : activePlugins )
    {
      std::filesystem::path pluginPath(pluginPathStr);
      std::filesystem::path pluginFileName(plugin);
      std::filesystem::path filename = pluginPath / pluginFileName;

      if ( LoadPlugin(filename.string() ) )
      {
        LOG(INFO, "Plugin [%s] loaded successfully", filename.string().c_str() );
      } 
    }
  }
  else
  {
    LOG(INFO,"CPluginManager::Initialize - plugin path empty - no plugins will be loaded")
  }
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
      tPrCreatePluginInstanceFunc componentGetter = reinterpret_cast<tPrCreatePluginInstanceFunc> (dlsym( fileHandle, "createInstance") );
      if ( componentGetter != nullptr )
      {
        std::unique_ptr<knocknock::IKnocknockPlugin> pluginInstance = componentGetter();
        if (pluginInstance)
        {
          LOG( INFO, "Loaded plugin: %s Version: %s", pluginInstance->GetName().c_str(), pluginInstance->GetVersion().c_str() );
          m_plugins.emplace_back(fileHandle, std::move(pluginInstance));
          // Successfully created plugin instance
          return true;
        }
      }
      else
      {
        LOG( ERROR, "Failed to find createInstance function in plugin: %s", pluginPath.c_str() );
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