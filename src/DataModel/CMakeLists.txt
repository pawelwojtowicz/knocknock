project( dbAccess )

SET( OUTPUT_NAME DataModel )

add_library( ${OUTPUT_NAME} STATIC
CApplication.cpp
CApplication.h
CApplicationParam.cpp
CApplicationParam.h
CPrivilege.cpp
CPrivilege.h
CRole.cpp
CRole.h
CUser.cpp
CUser.h
CUserSetting.cpp
CUserSetting.h
)

target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
