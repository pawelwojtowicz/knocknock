#pragma once
#include <string>
#include <map>
#include <optional>


namespace DBAccess
{
using tSystemParameters = std::map<std::string,std::string>; 

class ISystemParamData
{
public:
    virtual ~ISystemParamData() = default;

    // Example CRUD operations for system parameters
  virtual bool AddSystemParam( const std::string& key, const std::string& value) = 0;

  virtual tSystemParameters GetAllSystemParams()  = 0;

  virtual std::optional<std::string> GetSystemParam( const std::string& key) = 0;
};

}
