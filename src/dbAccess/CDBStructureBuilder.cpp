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

  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE ROLES ( NAME TEXT PRIMARY KEY NOT NULL, DESCRIPTION TEXT);", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE PRIVILEGES ( SHORT_DESC TEXT PRIMARY KEY NOT NULL, LONG_DESC TEXT);", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE SYSTEM_PARAMS ( VALUE_KEY TEXT PRIMARY KEY NOT NULL, VALUE TEXT);", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE USERS ( USER_ID TEXT PRIMARY KEY NOT NULL, FIRST_NAME TEXT, LAST_NAME TEXT, PASS_HASH);", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE USER2ROLES ( USER_ID TEXT NOT NULL, ROLE_NAME TEXT NOT NULL, FOREIGN KEY (USER_ID) REFERENCES USERS(USER_ID),FOREIGN KEY (ROLE_NAME) REFERENCES ROLES(NAME), PRIMARY KEY( USER_ID, ROLE_NAME));", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE ROLE2PRIVILEGE ( ROLE_NAME TEXT NOT NULL, PRIVILEGE_SHORT_DESC TEXT NOT NULL,FOREIGN KEY (ROLE_NAME) REFERENCES ROLES(NAME), FOREIGN KEY (PRIVILEGE_SHORT_DESC) REFERENCES PRIVILEGES(SHORT_DESC),PRIMARY KEY( ROLE_NAME, PRIVILEGE_SHORT_DESC));", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE APPLICATIONS ( APP_ID INTEGER NOT NULL UNIQUE, APP_NAME TEXT, APP_DATA_PUBLISHER TEXT, APP_ACCESS_TOKEN TEXT, PRIMARY KEY( APP_ID AUTOINCREMENT));", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE APPLICATION_PARAMS ( APPLICATION_ID INTEGER, NAME TEXT, PUBLIC INTEGER, VALUE TEXT, PRIMARY KEY( APPLICATION_ID, NAME), FOREIGN KEY(APPLICATION_ID) REFERENCES APPLICATIONS(APP_ID) );", executionCallback, 0 );
  m_rDBDriver.ExecuteSQLCommand( "CREATE TABLE USER_SETTINGS ( APPLICATION_ID INTEGER NOT NULL, APPLICATION_PARAM_NAME TEXT NOT NULL, USER_ID TEXT NOT NULL, VALUE TEXT, FOREIGN KEY (APPLICATION_ID) REFERENCES APPLICATIONS (APP_ID),FOREIGN KEY (APPLICATION_PARAM_NAME) REFERENCES APPLICATION_PARAMS (NAME), FOREIGN KEY (USER_ID) REFERENCES USERS (USER_ID),PRIMARY KEY(APPLICATION_ID,APPLICATION_PARAM_NAME,USER_ID ) );", executionCallback, 0 );
  return true;
}


}