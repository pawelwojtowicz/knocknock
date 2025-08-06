project( knockEngine )

SET( OUTPUT_NAME knockEngine )

add_library( ${OUTPUT_NAME} STATIC
CKnocknockService.cpp
CKnocknockService.h
CSession.cpp
CSession.h
IKnocknockService.h
)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} DataModel dbAccess)
target_link_libraries( ${OUTPUT_NAME} DataModel dbAccess sqlite3 )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})