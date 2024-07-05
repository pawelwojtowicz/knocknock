project( knockEngine )

SET( OUTPUT_NAME CryptoTools )

add_library( ${OUTPUT_NAME} STATIC
CAESCipherWrapper.cpp
CAESCipherWrapper.h
CAuthenticationChallengeTools.cpp
CAuthenticationChallengeTools.h
CHashFunction.cpp
CHashFunction.h
)

target_link_libraries( ${OUTPUT_NAME} cryptopp )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})