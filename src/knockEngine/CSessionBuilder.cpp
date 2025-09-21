#include "CSessionBuilder.h"
#include <CUser.h>
#include <CAuthenticationChallengeTools.h>

namespace knocknock
{
const std::string cAnonymousUserId = {"anonymous"}; 

CSessionBuilder::CSessionBuilder( CConfiguration& rConfiguration, DBAccess::IDBAccess& rDBAccess )
  : m_rConfiguration(rConfiguration)
  , m_rDBAccess(rDBAccess)
  , m_defaultAuthMethod{}
  , m_defaultAuthString{}
  , m_anonymousTemplate{}

{
}

CSessionBuilder::~CSessionBuilder()
{
}

bool CSessionBuilder::Initialize()
{
  m_defaultAuthMethod = m_rConfiguration.GetParamString(cParamName_DefaultAuthenticationMethod);
  m_defaultAuthString = m_rConfiguration.GetParamString(cParamName_DefaultAuthenticationString);
  m_anonymousTemplate = m_rConfiguration.GetParamString(cParamName_AnonymousUserTemplate);

  return ( !m_defaultAuthMethod.empty() && !m_defaultAuthString.empty() );
}

std::optional<CSession> CSessionBuilder::CreateSession(const std::string& userId)
{
  std::optional<knocknock::CUser> userInfo = m_rDBAccess.GetUserData().GetUserByUserId(userId);
  if ( !userInfo.has_value() )
  {
    if ( !m_anonymousTemplate.empty() )
    {
      std::optional<knocknock::CUser> userTemplate = m_rDBAccess.GetUserData().GetUserByUserId(m_anonymousTemplate);
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

  std::string userAuthMethod( m_defaultAuthMethod );
  if ( !userInfo->getAuthenticationMethod().empty() ) 
  {
    userAuthMethod = userInfo->getAuthenticationMethod(); 
  }

  std::string userAuthString( m_defaultAuthString );
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