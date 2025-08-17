#include "CSessionBuilder.h"
#include <CUser.h>
#include <CAuthenticationChallengeTools.h>

namespace knocknock
{
const std::string cAnonymousUserId = {"anonymous"}; 

CSessionBuilder::CSessionBuilder( CConfiguration& rConfiguration, DBAccess::IDBAccess& rDBAccess )
  : m_rConfiguration(rConfiguration)
  , m_rDBAccess(rDBAccess)
{
}

CSessionBuilder::~CSessionBuilder()
{
}

std::optional<CSession> CSessionBuilder::CreateSession(const std::string& userId)
{
  std::optional<knocknock::CUser> userInfo = m_rDBAccess.GetUserData().GetUserByUserId(userId);
  if ( !userInfo.has_value() )
  {
    if ( m_rConfiguration.GetAllowAnonymousLogin() )
    {
      CUser anonymousUserTemplate( userId, cAnonymousUserId,"",m_rConfiguration.GetDefaultAuthenticationMethod(), m_rConfiguration.GetDefaultAuthenticationString() );
      userInfo = std::move(anonymousUserTemplate);
    }
  }

  if (!userInfo.has_value())
  {
    return std::nullopt;
  }

  //generate the sessionId
  std::string sessionId = CAuthenticationChallengeTools::GenerateSessionId(userId);

  std::string userAuthMethod( m_rConfiguration.GetDefaultAuthenticationMethod() );
  if ( !userInfo->getAuthenticationMethod().empty() ) 
  {
    userAuthMethod = userInfo->getAuthenticationMethod(); 
  }

  std::string userAuthString( m_rConfiguration.GetDefaultAuthenticationString() );
  if ( !userInfo->getAuthenticationString().empty() ) 
  {
    userAuthString = userInfo->getAuthenticationString(); 
  }

  CSession newSession(  sessionId,
                        userInfo->getUserId(), 
                        userInfo->getFirstName() + " " + userInfo->getLastName(),
                        userAuthMethod, 
                        userAuthString);

  return newSession;
}

}