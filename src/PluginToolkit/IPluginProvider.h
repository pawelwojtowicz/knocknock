#pragma once
#include <vector>
#include <memory>
#include "IKnocknockPlugin.h"

namespace knocknock
{
class IPluginProvider
{
public:
  using tPluginTuple = std::tuple<void*, std::unique_ptr<IKnocknockPlugin>>;
  using tPluginList = std::vector<tPluginTuple>;

  IPluginProvider() = default;
  virtual ~IPluginProvider() = default;

  virtual const tPluginList& GetPlugins() const = 0;
};
}