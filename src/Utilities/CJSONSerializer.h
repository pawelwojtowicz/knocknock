#pragma once
#include "UtilityTypes.h"
#include <nlohmann/json.hpp>

namespace knocknock
{
using JSON = nlohmann::json;

class CJSONSerializer
{
public:
    CJSONSerializer( tKeyValueMap& rKeyValueMap );
    ~CJSONSerializer();

    // Delete copy and move operations
    CJSONSerializer(const CJSONSerializer&) = delete;
    CJSONSerializer& operator=(const CJSONSerializer&) = delete;
    CJSONSerializer(CJSONSerializer&&) = delete;
    CJSONSerializer& operator=(CJSONSerializer&&) = delete;

    bool Serialize(std::string& rOutput);
    bool Deserialize(const std::string& rInput);
private:
    bool BuildJSONModel(JSON& jsonModel, tStringList& keyTokens, const std::string& value);
    bool DetectVectorElement(const std::string& input, std::string& name, std::string& id);

private:
    tKeyValueMap& m_rKeyValueMap;
};
}