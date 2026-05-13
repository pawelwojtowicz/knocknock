project( knockEngine )

SET( OUTPUT_NAME CryptoTools )

add_library( ${OUTPUT_NAME} STATIC
CAESCipherWrapper.cpp
CAESCipherWrapper.h
CArgon2idWrapper.cpp
CArgon2idWrapper.h
CAuthenticationChallengeTools.cpp
CAuthenticationChallengeTools.h
CSHA256Hash.cpp
CSHA256Hash.h
)

find_package(OpenSSL REQUIRED)
target_link_libraries( ${OUTPUT_NAME} OpenSSL::SSL OpenSSL::Crypto argon2 )
target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})