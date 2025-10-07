#include "CProcessorRegistry.h"
#include "URLInfo.h"
#include "CURLParser.h"
#include "CSimpleMatcher.h"

#define METHOD 0
#define URLMATCHER 1
#define PROCESSOR 2

namespace HTTPServer
{

bool CProcessorRegistry::RegisterRequestProcessor( const HTTPServer::HttpMethod method, const std::string& urlPattern, std::shared_ptr<IRequestProcessor>& processor )
{
  m_processors.push_back( std::make_tuple( method, std::make_unique<CSimpleMatcher>(urlPattern), processor ) );

  return true;
}

bool CProcessorRegistry::ProcessRequest(  const HTTPServer::HttpMethod method, 
                                          const std::string& url, 
                                          const std::string& requestBody, 
                                          std::string& responseBody)
{
  URLInfo urlMetaData;
  if ( CURLParser::Parse( url , urlMetaData ) )
  {
    for ( const auto& processor : m_processors )
    {
      if ( ( std::get<METHOD>(processor) == method ) && std::get<URLMATCHER>(processor)->PathMatches( urlMetaData.path ) )
      {
        return std::get<PROCESSOR>(processor)->ProcessRequest( urlMetaData, requestBody, responseBody );
      }
    }
  } 
  return false;
}
}