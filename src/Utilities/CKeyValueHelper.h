#pragma once
#include "UtilityTypes.h"

namespace knocknock
{
class CKeyValueHelper
{
public:
  CKeyValueHelper(const tKeyValueMap& map);
  virtual ~CKeyValueHelper() = default;
  
  bool GetValue(const std::string& key, std::string& value) const;

private:
  const tKeyValueMap& m_map;
};


}