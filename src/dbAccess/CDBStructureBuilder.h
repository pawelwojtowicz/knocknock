#pragma once

namespace DBAccess
{
class IDBDriver;

class CDBStructureBuilder
{
public:
  CDBStructureBuilder( IDBDriver& );
  virtual ~CDBStructureBuilder();

  bool PrepareDatabaseStructure();

private:
  IDBDriver& m_rDBDriver;
};

}