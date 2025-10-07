#include <gtest/gtest.h>
#include <CURLParser.h>
#include <iostream>

std::string url( "https://user:pass@example.com:443/path/to/my%2dfile.txt?id=42&name=John%20Doe+Jingleheimer%2DSchmidt#page%20anchor");


TEST( CURLParserTest, basic ) 
{
  HTTPServer::URLInfo info;

  ASSERT_TRUE(HTTPServer::CURLParser::Parse(url, info ) );
  EXPECT_EQ( info.user , "user");
  EXPECT_EQ( info.password , "pass");
  EXPECT_EQ( info.path, "/path/to/my-file.txt");
  EXPECT_EQ( info.parameters[std::string("id")], std::string("42") );
  EXPECT_EQ( info.parameters[std::string("name")], std::string("John Doe Jingleheimer-Schmidt"));
}