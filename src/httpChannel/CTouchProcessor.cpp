#include "CTouchProcessor.h"
#include <CJSONSerializer.h>
#include <CSession.h>
#include "httpChannelConst.h"
#include "CCookieBuilder.h"

namespace knocknock
{
CTouchProcessor::CTouchProcessor(IKnocknockService& knocknockService, CCookieBuilder& cookieBuilder)
  : m_knocknockService(knocknockService)
  , m_cookieBuilder(cookieBuilder)
{
}

bool CTouchProcessor::ProcessRequest( const HTTPServer::URLInfo& urlInfo,
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
  const CSession session = m_knocknockService.Touch(input, output);

  switch( session.GetUserSessionState() )
  {
    case UserSessionState::INVALID_SESSION:
    case UserSessionState::AUTH_FAILED:
      requestResponse.responseCode = cHTTPResult_Unauthorized;
      output["message"] = "Invalid session";
      break;
    case UserSessionState::AUTH_SUCCESS:
      requestResponse.responseCode = cHTTPResult_OK;
      output["sessionId"] = session.GetSessionId();
      output["userId"] = session.GetUserId();
      output["userName"] = session.GetUserName();
      responseHeaders["Set-Cookie"] = m_cookieBuilder.BuildCookie(session);
      break;
    default:
      requestResponse.responseCode = cHTTPResult_InternalServerError;
      output["message"] = "Internal server error";  
      break;
  }

  responseHeaders["Content-Type"] = "application/json";
  CJSONSerializer responseSerializer(output);
  responseSerializer.Serialize(requestResponse.responseBody);


  return true; // Return true if processing is successful, false otherwise
}

} // namespace knocknock