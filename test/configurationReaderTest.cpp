#include <gtest/gtest.h>
#include <CConfiguration.h>

TEST(CConfiguration, LoadFromFile)
{
  knocknock::CConfiguration config;
  bool loadResult = config.LoadConfig("config/knocknock.conf");
  EXPECT_TRUE(loadResult);

  std::string anonymousUserTemplate = config.GetParamString("anonymousUserTemplate");
  EXPECT_EQ(anonymousUserTemplate, "");

  std::string defaultAuthMethod = config.GetParamString("defaultAuthenticationMethod");
  EXPECT_EQ(defaultAuthMethod, "sha256");

  std::string defaultAuthString = config.GetParamString("defaultAuthenticationString");
  EXPECT_EQ(defaultAuthString, "db89a15ca72c6c91a94c03e6b7973bbbf01b3e67988c9f79d6b764b36d913a66");

  std::string allowedAuthMethods = config.GetParamString("allowedAuthMethods");
  EXPECT_EQ(allowedAuthMethods, "simpledb,sha256,scr");
}