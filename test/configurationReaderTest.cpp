#include <gtest/gtest.h>
#include <CConfiguration.h>
#include <fstream>
#include <filesystem>

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

TEST(CConfiguration, CommentLinesAreSkipped)
{
  const std::string testFile = "test_comments.conf";
  {
    std::ofstream ofs(testFile);
    ofs << "# this is a comment\n";
    ofs << "key1=value1\n";
    ofs << "# another comment\n";
    ofs << "key2=value2\n";
    ofs << "\n";
    ofs << "key3=value3\n";
  }

  knocknock::CConfiguration config;
  EXPECT_TRUE(config.LoadConfig(testFile));

  EXPECT_EQ(config.GetParamString("key1"), "value1");
  EXPECT_EQ(config.GetParamString("key2"), "value2");
  EXPECT_EQ(config.GetParamString("key3"), "value3");
  EXPECT_EQ(config.GetParamString("# this is a comment"), "");
  EXPECT_EQ(config.GetParamString("# another comment"), "");

  std::filesystem::remove(testFile);
}