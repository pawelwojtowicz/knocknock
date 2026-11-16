cmake_minimum_required(VERSION 3.10)
project(knocKnock VERSION 0.0.99 DESCRIPTION "Identity Server for Embedded devices")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static libraries get linked into plugin shared objects (see MQTTConnectivityPlugin),
# so all code must be position-independent.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory( src )
add_subdirectory( test )

