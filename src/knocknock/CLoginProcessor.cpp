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
  tKeyValueMap input; // Populate this map with parsed credentials from requestBody
  tKeyValueMap output; 
  CJSONSerializer serializer(input);
  serializer.Deserialize(requestBody);

  for (const auto& pair : requestHeaders)
  {
    std::cout << "Input Key: " << pair.first << ", Value: " << pair.second << std::endl;
  }
  
  const CSession session = m_knocknockService.Login(input, output);

  std::cout << "Session ID: " << static_cast<int>(session.GetUserSessionState()) << std::endl;

  output["sessionId"] = session.GetSessionId();
  output["userId"] = session.GetUserId();
  output["userName"] = session.GetUserName();

  for (const auto& pair : output)
  {
    std::cout << "Output Key: " << pair.first << ", Value: " << pair.second << std::endl;
  }

  if ( session.GetUserSessionState() == UserSessionState::AUTH_SUCCESS )
  {
    
    CJSONSerializer responseSerializer(output);
    responseSerializer.Serialize(responseBody);
    return true;
  }

  std::cout << "Processing login request..." << std::endl;
  return true; // Return true if processing is successful, false otherwise
}

} // namespace knocknock