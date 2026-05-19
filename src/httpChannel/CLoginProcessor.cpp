#include "CLoginProcessor.h"
#include <CJSONSerializer.h>
#include <CSession.h>
#include "CCookieBuilder.h"
#include "httpChannelConst.h"

namespace knocknock
{
CLoginProcessor::CLoginProcessor(IKnocknockService& knocknockService, CCookieBuilder& cookieBuilder)
  : m_knocknockService(knocknockService), m_cookieBuilder(cookieBuilder)
{
}

bool CLoginProcessor::ProcessRequest( const HTTPServer::URLInfo& urlInfo,
                                      const HTTPServer::tHeadersMap& requestHeaders,    
                                      const std::string& requestBody, 
                                      HTTPServer::tHeadersMap& responseHeaders,
                                      HTTPServer::RequestResponse& requestResponse )
{
  //extract values from request body
  tKeyValueMap input;
  CJSONSerializer serializer(input);
  serializer.Deserialize(requestBody);

  tKeyValueMap output; 

  // Call the Login method of the IKnocknockService implementation
  const CSession session = m_knocknockService.Login(input, output);

  switch ( session.GetUserSessionState() )
  {
    case UserSessionState::INVALID_SESSION:
    case UserSessionState::AUTH_FAILED:
      requestResponse.responseCode = cHTTPResult_Unauthorized;
      output["message"] = "Invalid credentials";
      break;
    case UserSessionState::AUTH_IN_PROGRESS:
      requestResponse.responseCode = cHTTPResult_Created;
      output["sessionId"] = session.GetSessionId();

      break;
    case UserSessionState::AUTH_SUCCESS:
    {
      requestResponse.responseCode = cHTTPResult_OK;
      output["sessionId"] = session.GetSessionId();
      output["userId"] = session.GetUserId();
      output["userName"] = session.GetUserName();
      int id = 0;
      for ( const auto& role : session.GetRoles())
      {
        std::string roleKey = "roles[" + std::to_string(id++) +"]"; 
        output[roleKey] = role;
      }
      id = 0;
      for (const auto& privilege : session.GetPrivileges() )
      {
        std::string privilegeKey = "privileges[" + std::to_string(id++) + "]";
        output[privilegeKey] = privilege;
      }
      responseHeaders["Set-Cookie"] = m_cookieBuilder.BuildCookie(session);
    };break;
    default:
      requestResponse.responseCode = cHTTPResult_InternalServerError;
      output["message"] = "Internal server error";
      break;
  } 

  CJSONSerializer responseSerializer(output);
  responseSerializer.Serialize(requestResponse.responseBody);
  responseHeaders["Content-Type"] = "application/json";

  return true; // Return true if processing is successful, false otherwise
}

} // namespace knocknock