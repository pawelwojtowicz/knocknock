find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable( dbAccessTests dbAccessTests.cpp cryptoToolsTests.cpp authenticationToolsTests.cpp sessionBuilderTests.cpp authenticatorTests.cpp)

target_link_libraries(dbAccessTests DataModel dbAccess knockEngine cryptopp CryptoTools ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread )
add_dependencies( dbAccessTests DataModel dbAccess knockEngine CryptoTools)