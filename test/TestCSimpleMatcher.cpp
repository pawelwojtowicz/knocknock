#include <gtest/gtest.h>
#include <CSimpleMatcher.h>

TEST( CSimpleMatcherTest, matches ) 
{
  HTTPServer::CSimpleMatcher matcherUnderTest("/path/to/");
  HTTPServer::IURLMatcher& iMatcher( matcherUnderTest );
  
  ASSERT_TRUE( iMatcher.PathMatches( "/path/to/my-file.txt" ) );
}

TEST( CSimpleMatcherTest, doesNotMatch ) 
{
  HTTPServer::CSimpleMatcher matcherUnderTest("/path/to/");
  HTTPServer::IURLMatcher& iMatcher( matcherUnderTest );
  
  ASSERT_FALSE( iMatcher.PathMatches( "/pot/to/my-file.txt" ) );
}