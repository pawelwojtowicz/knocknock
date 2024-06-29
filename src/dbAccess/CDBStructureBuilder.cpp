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
  auto executionCallback = [](void *data, int argc, char **argv, char **azColName) {
    return 0;
  };

  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE SYSTEM_PARAMS ( VALUE_KEY TEXT PRIMARY KEY NOT NULL, VALUE TEXT);", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE USERS ( USER_ID TEXT PRIMARY KEY NOT NULL, FIRST_NAME TEXT, LAST_NAME TEXT, PASS_HASH);", executionCallback, 0 );

  return true;
}


}