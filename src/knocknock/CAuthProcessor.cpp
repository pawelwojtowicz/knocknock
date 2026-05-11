#include "CAuthProcessor.h"
#include <CJSONSerializer.h>
#include <CSession.h>
#include "KnocknockConst.h"

namespace knocknock
{
CAuthProcessor::CAuthProcessor(IKnocknockService& knocknockService)
  : m_knocknockService(knocknockService)
{
}

bool CAuthProcessor::ProcessRequest( const HTTPServer::URLInfo& urlInfo,
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

  const CSession session = m_knocknockService.Authenticate(input, output);

  switch ( session.GetUserSessionState() )
  {
    case UserSessionState::INVALID_SESSION:
    case UserSessionState::AUTH_FAILED:
      requestResponse.responseCode = cHTTPResult_Unauthorized;
      output["message"] = "Authentication failed";
      break;
    case UserSessionState::AUTH_IN_PROGRESS:
      requestResponse.responseCode = cHTTPResult_Created;
      output["sessionId"] = session.GetSessionId();
      break;
    case UserSessionState::AUTH_SUCCESS:
      requestResponse.responseCode = cHTTPResult_OK;
      output["sessionId"] = session.GetSessionId();
      output["userId"] = session.GetUserId();
      output["userName"] = session.GetUserName();
      break;
    default:
      requestResponse.responseCode = cHTTPResult_InternalServerError;
      output["message"] = "Internal server error";
      break;
  }

  CJSONSerializer responseSerializer(output);
  responseSerializer.Serialize(requestResponse.responseBody);
  responseHeaders["Content-Type"] = "application/json";

  return true;
}

} // namespace knocknock
