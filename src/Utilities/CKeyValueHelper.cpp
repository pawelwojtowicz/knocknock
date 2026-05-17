#include "CKeyValueHelper.h"

namespace knocknock
{
CKeyValueHelper::CKeyValueHelper(const tKeyValueMap& map)
: m_map(map)
{

}

bool CKeyValueHelper::GetValue(const std::string& key, std::string& value) const
{
  auto it = m_map.find(key);
  if (it != m_map.end())
  {
    value = it->second;
    return true;
  }
  return false;
}

}