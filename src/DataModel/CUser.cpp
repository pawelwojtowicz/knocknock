#include "CUser.h"

namespace knocknock
{

CUser::CUser( const std::string& userId, const std::string& userFirstName, const std::string& userLastName,const std::string& authMethod, const std::string& authString )
: m_userId{userId}
, m_firstName{userFirstName}
, m_lastName{userLastName}
, m_authMethod{authMethod}
, m_authString{authString}
{

}

const std::string& CUser::getUserId() const
{
  return m_userId;
}

const std::string& CUser::getFirstName() const
{
  return m_firstName;
}

const std::string& CUser::getLastName() const
{
  return m_lastName;
}

const std::string& CUser::getAuthenticationMethod() const
{
  return m_authMethod;
}

const std::string& CUser::getAuthenticationString() const
{
  return m_authString;
}

}