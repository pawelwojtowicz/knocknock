#include "CDatabase.h"
#include <filesystem>
#include "CDBStructureBuilder.h"

namespace DBAccess
{

CDatabase::CDatabase()
: m_dbDriver()
, m_sysParamData(m_dbDriver)
{

}

CDatabase::~CDatabase()
{
  
}

void CDatabase::OpenDatabase( const std::string& dbFilename )
{
  bool dbExisted ( std::filesystem::exists( dbFilename) );
  m_dbDriver.Open(dbFilename);

  if (!dbExisted)
  {
    CDBStructureBuilder dbBuilder( m_dbDriver );
    dbBuilder.PrepareDatabaseStructure();
  }
}

void CDatabase::Close()
{
  m_dbDriver.Close();

}

}