#pragma once
#include <string>

namespace knocknock
{
  
class IKnockKnockPlugin
{
public:
  IKnockKnockPlugin() = default;
  virtual ~IKnockKnockPlugin() = default;

  virtual std::string GetName() const = 0;
  virtual std::string GetVersion() const = 0;

};

}

