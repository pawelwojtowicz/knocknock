project( dbAccess )

SET( OUTPUT_NAME dbAccess )

add_library( ${OUTPUT_NAME} STATIC
CDatabase.cpp
CDatabase.h
CDBStructureBuilder.cpp
CDBStructureBuilder.h
CSQLiteDriver.cpp
CSQLiteDriver.h
CSystemParamData.cpp
CSystemParamData.h
CUserData.cpp
CUserData.h
IDBDriver.h
)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} DataModel )
target_link_libraries( ${OUTPUT_NAME} DataModel sqlite3 )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#install( TARGETS ${OUTPUT_NAME} RUNTIME DESTINATION lib )