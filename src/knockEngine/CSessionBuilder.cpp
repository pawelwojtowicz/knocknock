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
    if ( !m_rConfiguration.GetAnonymousUserTemplate().empty() )
    {
      std::optional<knocknock::CUser> userTemplate = m_rDBAccess.GetUserData().GetUserByUserId( m_rConfiguration.GetAnonymousUserTemplate() );
      if ( userTemplate.has_value() )
      {
        const CUser& userTemplateRef = userTemplate.value();
        userInfo = CUser(userId, userTemplateRef.getFirstName(), userTemplateRef.getLastName(), userTemplateRef.getAuthenticationMethod(), userTemplateRef.getAuthenticationString() );
      }
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