project( MQTTConnectivityPlugin )

SET( OUTPUT_NAME mqttConnectivityPlugin )

add_library( ${OUTPUT_NAME} SHARED
PluginCreator.cpp
CMQTTConnectivityPlugin.cpp
CMQTTConnectivityPlugin.h)

#set_target_properties(${OUTPUT_NAME} PROPERTIES VERSION ${HOME_AUTOMATION_SERVICES_VERSION} SOVERSION ${HOME_AUTOMATION_SERVICES_VERSION} )
add_dependencies( ${OUTPUT_NAME} knockEngine)
target_link_libraries( ${OUTPUT_NAME} knockEngine )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})