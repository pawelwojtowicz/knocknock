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

TEST(CTokenizerTest, TokenizeTrimmed_StripsWhitespaceAndDropsEmpty)
{
    knocknock::tStringList expected = {"simpledb", "sha256", "scr"};
    EXPECT_EQ(knocknock::CTokenizer::TokenizeTrimmed(" simpledb, sha256 ,,\tscr ,\r", ','), expected);
    EXPECT_TRUE(knocknock::CTokenizer::TokenizeTrimmed("", ',').empty());
    EXPECT_TRUE(knocknock::CTokenizer::TokenizeTrimmed(" , ", ',').empty());
}

TEST(CTokenizerTest, Trim)
{
    EXPECT_EQ(knocknock::CTokenizer::Trim("  value\r"), "value");
    EXPECT_EQ(knocknock::CTokenizer::Trim("a b"), "a b");
    EXPECT_EQ(knocknock::CTokenizer::Trim(" \t\r\n"), "");
    EXPECT_EQ(knocknock::CTokenizer::Trim(""), "");
}
