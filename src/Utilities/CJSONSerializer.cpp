#include "CJSONSerializer.h"
#include "CTokenizer.h"

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
  JSON jsonModel = {};
  for (const auto& keyValuePair : m_rKeyValueMap)
  {
    knocknock::tStringList keyTokens = CTokenizer::Tokenize(keyValuePair.first, '.');
    BuildJSONModel(jsonModel, keyTokens, keyValuePair.second);
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
  JSON jsonModel;
  try
  {
    jsonModel = JSON::parse(rInput);
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

bool CJSONSerializer::DetectVectorElement(const std::string& input, std::string& name, std::string& id) 
{
  size_t openBracket = input.find('[');
  size_t closeBracket = input.find(']', openBracket);
  
  // Check that brackets exist and are in the right order
  if (openBracket == std::string::npos || closeBracket == std::string::npos || 
      closeBracket <= openBracket + 1) {
      return false;
  }
  
  // Ensure there is no extra '[' or ']' before/after (optional)
  // Extract name (everything before '[')
  name = input.substr(0, openBracket);
  // Extract id (between brackets)
  id = input.substr(openBracket + 1, closeBracket - openBracket - 1);
  
  // Optionally verify that nothing follows after ']'
  if (closeBracket + 1 != input.length()) {
      return false;  // extra characters after the closing bracket
  }
  
  return !name.empty() && !id.empty();
}

bool CJSONSerializer::BuildJSONModel(JSON& jsonModel, tStringList& keyTokens, const std::string& value)
{
  std::string keyString = keyTokens.front();
  keyTokens.pop_front();

  int arrayIndex = -1;
  std::string name, id;
  
  if (DetectVectorElement(keyString, name, id) )
  {
    arrayIndex = std::stoi(id);
    if (!jsonModel.contains(name) || !jsonModel[name].is_array())
    {
      jsonModel[name] = JSON::array();
    }
    if (jsonModel[name].size() <= arrayIndex)
    {
      jsonModel[name].get_ref<std::vector<JSON>&>().resize(arrayIndex + 1);
    }
    if (keyTokens.empty())
    {
      jsonModel[name][arrayIndex] = value;
    }
    else
    {
      BuildJSONModel(jsonModel[name][arrayIndex], keyTokens, value);
    }
  }
  else
  {
    if (keyTokens.empty())
    {
      jsonModel[keyString] = value;
    }
    else
    {
      if (!jsonModel.contains(keyString) || !jsonModel[keyString].is_object())
      {
        jsonModel[keyString] = JSON::object();
      }
      BuildJSONModel(jsonModel[keyString], keyTokens, value);
    }
  }
    // Implementation for building JSON model
  return true;
}

}