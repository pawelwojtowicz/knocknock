#pragma once
#include <string>
#include <vector>

namespace knocknock
{
class CUser
{
public:
  CUser() = default;
  CUser( const std::string& userId, const std::string& userFirstName, const std::string& userLastName,const std::string& authMethod, const std::string& authString );

  const std::string& getUserId() const;
  const std::string& getFirstName() const;
  const std::string& getLastName() const;
  const std::string& getAuthenticationMethod() const;
  const std::string& getAuthenticationString() const;

private:
  std::string m_userId = {};
  std::string m_firstName = {};
  std::string m_lastName = {};
  std::string m_authMethod = {};
  std::string m_authString = {};
};

using tUserArray = std::vector<CUser>;
}