#include "CDatabase.h"
#include <filesystem>

namespace DBAccess
{

CDatabase::CDatabase()
: m_dbDriver(std::make_unique<CSQLiteDriver>())
{

}

CDatabase::~CDatabase()
{
  
}

void CDatabase::OpenDatabase( const std::string& dbFilename )
{
  bool dbExisted ( std::filesystem::exists( dbFilename) );
  m_dbDriver->Open(dbFilename);

  if (!dbExisted)
  {
    //initialize the DB structure.
  }
}

void CDatabase::Close()
{

}


}