project( Utilities )

SET( OUTPUT_NAME Utilities )

add_library( ${OUTPUT_NAME} STATIC
CConfiguration.cpp
CConfiguration.h
CJSONSerializer.cpp
CJSONSerializer.h
CKeyValueHelper.cpp
CKeyValueHelper.h
CTimespan.cpp
CTimespan.h
)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} DataModel dbAccess )
target_link_libraries( ${OUTPUT_NAME} DataModel dbAccess sqlite3 )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})