project( dbAccess )

SET( OUTPUT_NAME DataModel )

add_library( ${OUTPUT_NAME} STATIC
CUser.cpp
CUser.h
)

target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
