project (knocknock CXX)

SET( OUTPUT_NAME knocknock )

add_executable ( ${OUTPUT_NAME}
CMain.cpp
CMain.h
main.cpp
)

target_link_libraries ( ${OUTPUT_NAME} Logger DataModel dbAccess knockEngine CryptoTools Utilities )
add_dependencies( ${OUTPUT_NAME} Logger DataModel dbAccess knockEngine CryptoTools Utilities)
#install ( TARGETS ${OUTPUT_NAME} RUNTIME DESTINATION ${ECUAPP_RUNTIME_INSTALL_DIR} )