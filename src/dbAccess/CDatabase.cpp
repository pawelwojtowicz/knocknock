#include "CDatabase.h"
#include <filesystem>
#include "CDBStructureBuilder.h"


namespace DBAccess
{

CDatabase::CDatabase( IDBDriver& rDBDriver )
: m_rDBDriver(rDBDriver)
, m_applicationData(m_rDBDriver)
, m_applicationParamData(m_rDBDriver)
, m_privilegeData(m_rDBDriver)
, m_roleData(m_rDBDriver)
, m_role2PrivilegeMappingData(m_rDBDriver)
, m_sysParamData(m_rDBDriver)
, m_userData(m_rDBDriver)
, m_user2RoleMappingData(m_rDBDriver)
, m_userSettingsData(m_rDBDriver)
{
}

void CDatabase::OpenDatabase( const std::string& dbFilename )
{
  bool dbExisted ( std::filesystem::exists( dbFilename) );
  m_rDBDriver.Open(dbFilename);

  if (!dbExisted)
  {
    CDBStructureBuilder dbBuilder( m_rDBDriver );
    dbBuilder.PrepareDatabaseStructure();
  }
}

void CDatabase::Close()
{
  m_rDBDriver.Close();

}

}