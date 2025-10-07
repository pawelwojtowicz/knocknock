#include "CURLParser.h"

#include <boost/system/errc.hpp>
#include <boost/system/result.hpp>
#include <boost/url/url.hpp>
#include <boost/url/parse.hpp>
#include <boost/url/parse_path.hpp>
#include <boost/url/string_view.hpp>

#include <iostream>

namespace urls = boost::urls;

namespace HTTPServer
{
bool CURLParser::Parse( const std::string& url, URLInfo& urlInfo)
{
  boost::system::result<urls::url_view> urlParsingResult = urls::parse_uri_reference( url );
  if ( urlParsingResult )
  {
    urlInfo.path = urlParsingResult->path();
    urlInfo.user = urlParsingResult->user();
    urlInfo.password = urlParsingResult->password();
    for( const auto& param : urlParsingResult->params() )
    {
      urlInfo.parameters[param.key] = param.value;
    }
    return true;
  }
  else
  {
      boost::system::error_code e = urlParsingResult.error();
  }

  return false;
}

}