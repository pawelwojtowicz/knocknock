#include "CJSONSerializer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace knocknock
{
CJSONSerializer::CJSONSerializer(tKeyValueMap& rKeyValueMap)
    : m_rKeyValueMap(rKeyValueMap)
{
}

CJSONSerializer::~CJSONSerializer()
{
    // TODO: Cleanup JSON serializer resources
}

bool CJSONSerializer::Serialize(std::string& rOutput)
{
  json jsonModel = {};
  for (const auto& keyValuePair : m_rKeyValueMap)
  {
    jsonModel[keyValuePair.first] = keyValuePair.second;
  }
  try 
  {
    rOutput = jsonModel.dump();
  }
  catch (const std::exception& e)
  {
    return false;
  }
  return true;
}

bool CJSONSerializer::Deserialize(const std::string& rInput)
{
  m_rKeyValueMap.clear(); // Clear existing data before deserialization
  json jsonModel;
  try
  {
    jsonModel = json::parse(rInput);
  }
  catch (const std::exception& e)
  {
    return false;
  }

  for (const auto &item : jsonModel.items())
  {
    if (item.value().is_string())
    {
      m_rKeyValueMap[item.key()] = item.value().get<std::string>();
    }
    else if (!item.value().is_null() && !item.value().is_structured())
    {
      m_rKeyValueMap[item.key()] = item.value().dump();
    }
  }
  
  return true;
}
}