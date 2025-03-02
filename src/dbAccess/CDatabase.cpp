#include "CDatabase.h"
#include <filesystem>
#include "CDBStructureBuilder.h"

namespace DBAccess
{

CDatabase::CDatabase()
: m_dbDriver()
, m_applicationData(m_dbDriver)
, m_applicatonParamData(m_dbDriver)
, m_privilegeData(m_dbDriver)
, m_roleData(m_dbDriver)
, m_role2PrivilegeMappingData(m_dbDriver)
, m_sysParamData(m_dbDriver)
, m_userData(m_dbDriver)
, m_user2RoleMappingData(m_dbDriver)
, m_userSettingsData(m_dbDriver)
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