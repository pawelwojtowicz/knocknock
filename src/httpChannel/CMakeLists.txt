project( httpChannel )

SET( OUTPUT_NAME httpChannel )

add_library( ${OUTPUT_NAME} STATIC
  CAuthProcessor.cpp
  CAuthProcessor.h
  CCookieBuilder.cpp
  CCookieBuilder.h
  CLoginProcessor.cpp
  CLoginProcessor.h
  CLogoutProcessor.cpp
  CLogoutProcessor.h
  CTouchProcessor.cpp
  CTouchProcessor.h
  CHTTPChannel.cpp
  CHTTPChannel.h
)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} HTTPRestSrv knockEngine Utilities)
target_link_libraries( ${OUTPUT_NAME} HTTPRestSrv knockEngine Utilities)
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})