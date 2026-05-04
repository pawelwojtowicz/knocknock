#include "CLoginProcessor.h"
#include <iostream>
#include <CJSONSerializer.h>
#include <CSession.h>

namespace knocknock
{
CLoginProcessor::CLoginProcessor(IKnocknockService& knocknockService)
  : m_knocknockService(knocknockService)
{
}

bool CLoginProcessor::ProcessRequest( const HTTPServer::URLInfo& urlInfo,
                                      const std::map<std::string, std::string>& requestHeaders,    
                                      const std::string& requestBody, 
                                      std::map<std::string, std::string>& responseHeaders,
                                      std::string& responseBody )
{
  //extract values from request body
  tKeyValueMap input;
  CJSONSerializer serializer(input);
  serializer.Deserialize(requestBody);

  tKeyValueMap output; 

  // Call the Login method of the IKnocknockService implementation
  const CSession session = m_knocknockService.Login(input, output);

  if ( session.GetUserSessionState() == UserSessionState::AUTH_SUCCESS )
  {
    output["sessionId"] = session.GetSessionId();
    output["userId"] = session.GetUserId();
    output["userName"] = session.GetUserName();

    CJSONSerializer responseSerializer(output);
    responseSerializer.Serialize(responseBody);
    responseHeaders["Content-Type"] = "application/json";
  }

  return true; // Return true if processing is successful, false otherwise
}

} // namespace knocknock