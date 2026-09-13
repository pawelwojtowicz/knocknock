#pragma once
#include <string>

namespace knocknock
{
  
class IKnocknockPlugin
{
public:
  IKnocknockPlugin() = default;
  virtual ~IKnocknockPlugin() = default;

  virtual std::string GetName() const = 0;
  virtual std::string GetVersion() const = 0;

};

}

