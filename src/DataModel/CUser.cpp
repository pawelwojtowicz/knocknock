#include "CUser.h"

namespace knocknock
{

User::User()
: m_userId{}
, m_firstName{}
, m_lastName{}
, m_passwordHash{}
{
}

User::User( const std::string& userId, const std::string& userName, const std::string& userLastName,const std::string& passHash )
: m_userId{userId}
, m_firstName{userName}
, m_lastName{userLastName}
, m_passwordHash{passHash}

{

}

const std::string& User::getUserId() const
{
  return m_userId;
}

const std::string& User::getFirstName() const
{
  return m_firstName;
}

const std::string& User::getLastName() const
{
  return m_lastName;
}
const std::string& User::getPasswordHash() const
{
  return m_passwordHash;
}

}