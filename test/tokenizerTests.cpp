#include "gtest/gtest.h"
#include <CTokenizer.h>

TEST(CTokenizerTest, TokenizeBasic)
{
    std::string input = "Hello,World,Test";
    char delimiter = ',';
    knocknock::tStringList expected = {"Hello", "World", "Test"};
    knocknock::tStringList result = knocknock::CTokenizer::Tokenize(input, delimiter);
    EXPECT_EQ(result, expected);
}

TEST(CTokenizerTest, TokenizeSingleElement)
{
    std::string input = "Hello";
    char delimiter = ',';
    knocknock::tStringList expected = {"Hello"};
    knocknock::tStringList result = knocknock::CTokenizer::Tokenize(input, delimiter);
    EXPECT_EQ(result, expected);
    EXPECT_EQ(result.size(), 1);
}