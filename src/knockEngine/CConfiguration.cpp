#include "CConfiguration.h"
#include <filesystem>
#include <fstream>

namespace knocknock
{

CConfiguration::CConfiguration()
{
  m_parameters[cParamName_AnonymousUserTemplate] = "";
  m_parameters[cParamName_DefaultAuthenticationMethod] = "sha256";
  m_parameters[cParamName_DefaultAuthenticationString] = "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66";
  m_parameters[cParamName_AllowedAuthMethods] = "simpledb,sha256,scr";
}

bool CConfiguration::LoadConfig(DBAccess::IDBAccess& rDBAccess)
{
  const auto systemParams = rDBAccess.GetSystemParamData().GetAllSystemParams();
  for (const auto& param : systemParams)
  {
    m_parameters[param.first] = param.second;
  }


  return true;
}

bool CConfiguration::LoadConfig(const std::string& filename)
{
  if ( !std::filesystem::exists(filename)  )
  {
    return false;
  }

  std::ifstream configurationFile(filename);
  std::string line{};
  while (std::getline(configurationFile, line))
  {
    auto pos = line.find('=');
    if (pos == std::string::npos)
    {
      continue; // Invalid line, skip
    }

    std::string paramName = line.substr(0, pos);
    std::string paramValue = line.substr(pos + 1);
    m_parameters[paramName] = paramValue;
  }
  configurationFile.close();
  return true;
}




const std::string CConfiguration::GetParamString( const std::string& paramName ) const
{
  tParameterMapCI it = m_parameters.find(paramName);
  if (it != m_parameters.end())
  {
    return it->second;
  }
  return {};
}


}