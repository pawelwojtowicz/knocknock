#pragma once
#include <string>
#include <vector>
#include <functional>

namespace DBAccess
{

//callback from the driver is the SQLITE thingie - will be changed sometime, or not.
using tSQLiteCallback = std::function<int(std::vector<std::string>)>;
//typedef int (tSQLiteCallback)(void *data, int argc, char **argv, char **azColName);

class IDBDriver
{
public:
  IDBDriver() = default;
  virtual ~IDBDriver() = default;

  virtual bool Open( const std::string& dbFilename) = 0;
  virtual void Close() = 0;

  virtual bool ExecuteSQLCommand( const std::string& command, tSQLiteCallback callbackFunction ) = 0;
};

}