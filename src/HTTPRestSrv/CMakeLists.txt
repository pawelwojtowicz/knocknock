project(HTTPRestSrv CXX)


add_library(HTTPRestSrv SHARED
  CHTTPRestController.cpp 
  CHTTPRestController.h 
  CHTTPConnection.cpp 
  CHTTPConnection.h
  CProcessorRegistry.cpp
  CProcessorRegistry.h
  CSimpleMatcher.cpp
  CSimpleMatcher.h
  CURLParser.cpp 
  CURLParser.h
  HTTPServerConfig.h
  URLInfo.h
)
target_link_libraries( HTTPRestSrv boost_system boost_thread boost_url pthread)
target_include_directories(HTTPRestSrv INTERFACE .)