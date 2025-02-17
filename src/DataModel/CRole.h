#pragma once
#include <string>
#include <vector>

namespace knocknock
{

class CRole
{
public:
  CRole() = default;
  CRole(const std::string& name, const std::string& description);
  ~CRole() = default;

  void SetName(const std::string& name) 
  {
    m_name = name;
  }

  void SetDescription( const std::string& description)
  {
    m_description = description;
  }

  const std::string& GetName() const 
  {
    return m_name;
  }

  const std::string& GetDescription() const
  {
    return m_description;
  }

private:
  std::string m_name;
  std::string m_description;
};

using tRoles = std::vector<CRole>;
}