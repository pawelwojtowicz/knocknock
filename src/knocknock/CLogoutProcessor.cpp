#include "CLogoutProcessor.h"
#include <CJSONSerializer.h>
#include <CSession.h>
#include "KnocknockConst.h"

namespace knocknock
{
CLogoutProcessor::CLogoutProcessor(IKnocknockService& knocknockService)
  : m_knocknockService(knocknockService)
{
}

bool CLogoutProcessor::ProcessRequest( const HTTPServer::URLInfo& urlInfo,
                                      const HTTPServer::tHeadersMap& requestHeaders,    
                                      const std::string& requestBody, 
                                      HTTPServer::tHeadersMap& responseHeaders,
                                      HTTPServer::RequestResponse& requestResponse )
{
  //extract values from request body
  tKeyValueMap input;
  tKeyValueMap output;
  CJSONSerializer serializer(input);
  serializer.Deserialize(requestBody);

  if ( m_knocknockService.Logout(input, output) )
  {
    requestResponse.responseCode = cHTTPResult_OK;
    output["message"] = "Logout successful";
  }
  else
  {
    requestResponse.responseCode = cHTTPResult_Unauthorized;
    output["message"] = "Invalid session";
  }

  CJSONSerializer responseSerializer(output);
  responseSerializer.Serialize(requestResponse.responseBody);

  return true; // Return true if processing is successful, false otherwise
}

} // namespace knocknock