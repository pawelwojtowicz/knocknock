#include "CDBStructureBuilder.h"
#include "IDBDriver.h"

namespace DBAccess
{
CDBStructureBuilder::CDBStructureBuilder( IDBDriver& rDBDriver)
: m_rDBDriver( rDBDriver )
{

}

CDBStructureBuilder::~CDBStructureBuilder()
{

}

bool CDBStructureBuilder::PrepareDatabaseStructure()
{
//  static constexpr char createSystemVarTable[] = {
//    "CREATE TABLE SYSTEM("  \
//    "KEY TEXT PRIMARY KEY     NOT NULL," \
//    "VALUE TEXT );"};

  auto executionCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE SYSTEM_PARAMS ( VALUE_KEY TEXT PRIMARY KEY NOT NULL, VALUE TEXT);", executionCallback, 0 );

  return true;
}


}