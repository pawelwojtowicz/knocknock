project( knockEngine )

SET( OUTPUT_NAME knockEngine )

add_library( ${OUTPUT_NAME} STATIC
CAuthenticator.cpp
CAuthenticator.h
CAuthMethodFactory.cpp
CAuthMethodFactory.h
CConfiguration.cpp
CConfiguration.h
CSessionManager.cpp
CSessionManager.h
CSCRAuthMethod.cpp
CSCRAuthMethod.h
CSession.cpp
CSession.h
CSessionBuilder.cpp
CSessionBuilder.h
IAuthenticationMethod.h
IKnocknockService.h
CSHA256AuthMethod.cpp
CSHA256AuthMethod.h
CSimpleDBAuthMethod.cpp
CSimpleDBAuthMethod.h
)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} DataModel dbAccess CryptoTools)
target_link_libraries( ${OUTPUT_NAME} DataModel dbAccess CryptoTools sqlite3 )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})