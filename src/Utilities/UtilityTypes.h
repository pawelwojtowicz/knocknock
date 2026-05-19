#pragma once
#include <map>
#include <list>
#include <string>

namespace knocknock
{
using tStringList = std::list<std::string>;
using tKeyValueMap = std::map<std::string, std::string>;
using tKeyValueMapIterator = tKeyValueMap::iterator;
using tKeyValueMapConstIterator = tKeyValueMap::const_iterator;
}