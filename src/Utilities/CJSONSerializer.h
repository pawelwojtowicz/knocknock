#pragma once
#include "UtilityTypes.h"

namespace knocknock
{

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
    tKeyValueMap& m_rKeyValueMap;
};
}