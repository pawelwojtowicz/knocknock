project( dbDriver )

SET( OUTPUT_NAME dbDriver )

add_library( ${OUTPUT_NAME} STATIC
CSQLiteDriver.cpp
CSQLiteDriver.h
IDBDriver.h
)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
target_link_libraries( ${OUTPUT_NAME} libsqlite3 )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#install( TARGETS ${OUTPUT_NAME} RUNTIME DESTINATION lib )