project( PluginToolkit )

SET( OUTPUT_NAME PluginToolkit )

add_library( ${OUTPUT_NAME} STATIC
CSession.cpp
CSession.h
IAuthenticationMethod.h
IKnocknockPlugin.h
ISessionPublisher.h
KnocKnockDictionary.h
knocknockTypes.h
PluginManifest.h
)

target_compile_features( ${OUTPUT_NAME} INTERFACE cxx_std_17)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} Utilities )
target_link_libraries( ${OUTPUT_NAME} Utilities )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})