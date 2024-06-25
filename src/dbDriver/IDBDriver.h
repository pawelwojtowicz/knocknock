#pragma once
#include <string>

namespace DBDriver
{

class IDBDriver
{
public:
  IDBDriver() = default;
  virtual ~IDBDriver() = default;

  virtual bool Open( const std::string& dbFilename) = 0;
  virtual void Close() = 0;

};

}