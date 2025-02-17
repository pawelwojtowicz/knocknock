#pragma once
#include <string>
#include <vector>

namespace knocknock
{
class CPrivilege
{
public:
  CPrivilege() = default;
  CPrivilege( const std::string& shortDesc, const std::string& longDesc);
  ~CPrivilege() = default;

  void SetShortDesc( const std::string& shortDesc) {
    m_shortDesc = shortDesc;
  }

  void SetLongDesc( const std::string& longDesc)
  {
    m_longDesc = longDesc;
  }

  const std::string& GetShortDesc() const {
    return m_shortDesc;
  }

  const std::string& GetLongDesc() const {
    return m_longDesc;
  }

private:
  std::string m_shortDesc;
  std::string m_longDesc;
};

using tPrivilegeArray = std::vector<CPrivilege>;
}