#include "CAuthProcessor.h"
#include <iostream>
#include <CJSONSerializer.h>
#include <CSession.h>

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

  return true; // Return true if processing is successful, false otherwise
}

} // namespace knocknock