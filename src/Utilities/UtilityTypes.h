#pragma once
#include <map>
#include <string>

namespace knocknock
{
using tKeyValueMap = std::map<std::string, std::string>;
using tKeyValueMapIterator = tKeyValueMap::iterator;
using tKeyValueMapConstIterator = tKeyValueMap::const_iterator;


}