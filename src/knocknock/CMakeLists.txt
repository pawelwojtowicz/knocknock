project (knocknock CXX)

SET( OUTPUT_NAME knocknock )

add_executable ( ${OUTPUT_NAME}
CPluginManager.cpp
CPluginManager.h
CMain.cpp
CMain.h
main.cpp
)

target_link_libraries ( ${OUTPUT_NAME} Logger DataModel dbAccess knockEngine CryptoTools Utilities httpChannel)
add_dependencies( ${OUTPUT_NAME} Logger DataModel dbAccess knockEngine CryptoTools Utilities httpChannel)
#install ( TARGETS ${OUTPUT_NAME} RUNTIME DESTINATION ${ECUAPP_RUNTIME_INSTALL_DIR} )