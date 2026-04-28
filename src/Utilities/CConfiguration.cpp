#include "CConfiguration.h"
#include <filesystem>
#include <fstream>

namespace knocknock
{

CConfiguration::CConfiguration()
{
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




const std::string CConfiguration::GetParamString( const std::string& paramName, const std::string defaultValue) const
{
  tParameterMapCI it = m_parameters.find(paramName);
  if (it != m_parameters.end())
  {
    return it->second;
  }
  return defaultValue;
}

int CConfiguration::GetParamInt( const std::string& paramName, int defaultValue ) const
{
  tParameterMapCI it = m_parameters.find(paramName);
  if (it != m_parameters.end())
  {
    try
    {
      return std::stoi(it->second);
    }
    catch (const std::exception& ex)
    {
      // Log the error and return default value
      // For example: LOG_ERROR("Invalid value for parameter " + paramName + ": " + it->second + ". Using default value: " + std::to_string(defaultValue));
    }
  }
  return defaultValue;
}



}