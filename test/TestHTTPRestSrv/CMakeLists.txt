enable_testing()
add_executable( tstHTTPRestSrv
TestCURLParser.cpp
TestCSimpleMatcher.cpp
TestCProcessorRegistry.cpp
)

target_link_libraries(tstHTTPRestSrv HTTPRestSrv ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} gmock pthread )
add_dependencies( tstHTTPRestSrv tstHTTPRestSrv )
add_test(  tstHTTPRestSrv tstHTTPRestSrv )