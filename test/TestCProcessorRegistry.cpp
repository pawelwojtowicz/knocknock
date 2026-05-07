#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <URLInfo.h>
#include <IRequestProcessor.h>
#include <CProcessorRegistry.h>
#include <IServiceContext.h>

using ::testing::_;
using ::testing::Return;

using namespace HTTPServer;

typedef const tHeadersMap& ConstMapRef;
typedef tHeadersMap& MutableMapRef;

class CProcessorMock : public IRequestProcessor
{
public:
  MOCK_METHOD(bool, ProcessRequest, (const URLInfo& urlInfo,
                                      ConstMapRef requestHeaders, 
                                      const std::string& requestBody, 
                                      MutableMapRef responseHeaders,
                                      std::string& responseBody), (override));
};

class TestCProcessorRegistry : public ::testing::Test
{
public:
  void SetUp()
  {
    processorA = std::make_shared<CProcessorMock>();
    processorB = std::make_shared<CProcessorMock>();
    processorC = std::make_shared<CProcessorMock>();

    std::shared_ptr<IRequestProcessor> iProcessorA = processorA;
    std::shared_ptr<IRequestProcessor> iProcessorB = processorB;
    std::shared_ptr<IRequestProcessor> iProcessorC = processorC;

    processorRegistryUnderTest.RegisterRequestProcessor( HttpMethod::mthd_POST, std::string("/path/2/A"), iProcessorA);
    processorRegistryUnderTest.RegisterRequestProcessor( HttpMethod::mthd_GET, std::string("/path/2/B"), iProcessorB);
    processorRegistryUnderTest.RegisterRequestProcessor( HttpMethod::mthd_DELETE, std::string("/path/2/C"), iProcessorC);
  }

  std::shared_ptr<CProcessorMock> processorA;
  std::shared_ptr<CProcessorMock> processorB;
  std::shared_ptr<CProcessorMock> processorC;
  HTTPServer::CProcessorRegistry processorRegistryUnderTest;
};

TEST_F( TestCProcessorRegistry , Basic_CaseA_allGood )
{
  EXPECT_CALL( *processorA, ProcessRequest( _, _, _, _, _ ) ).WillRepeatedly(Return(true));
  EXPECT_CALL( *processorB, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorC, ProcessRequest( _, _, _, _, _ ) ).Times(0);

  std::string url( "https://user:pass@example.com:443/path/2/A");
  std::string requestBody("");
  std::string responseBody("");

  IServiceContext& processor( processorRegistryUnderTest );

  tHeadersMap requestHeaders;
  tHeadersMap responseHeaders;
  
  ASSERT_TRUE( processor.ProcessRequest(  HttpMethod::mthd_POST, url, requestHeaders , requestBody, responseHeaders, responseBody ) ); 
}

TEST_F( TestCProcessorRegistry , Basic_CaseA_methodNotFit )
{
  EXPECT_CALL( *processorA, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorB, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorC, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  
  std::string url( "https://user:pass@example.com:443/path/2/A");
  std::string requestBody("");
  std::string responseBody("");
  tHeadersMap requestHeaders;
  tHeadersMap responseHeaders;

  IServiceContext& processor( processorRegistryUnderTest );
  
  ASSERT_FALSE( processor.ProcessRequest(  HttpMethod::mthd_GET, url, requestHeaders , requestBody, responseHeaders, responseBody ) ); 
}

TEST_F( TestCProcessorRegistry , Basic_CaseB_allGood )
{
  EXPECT_CALL( *processorA, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorB, ProcessRequest( _, _, _, _, _ ) ).Times(1).WillOnce(Return(true));
  EXPECT_CALL( *processorC, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  
  std::string url( "https://user:pass@example.com:443/path/2/B");
  tHeadersMap requestHeaders;
  tHeadersMap responseHeaders;
  std::string requestBody("");
  std::string responseBody("");

  IServiceContext& processor( processorRegistryUnderTest );
  
  ASSERT_TRUE( processor.ProcessRequest(  HttpMethod::mthd_GET, url, requestHeaders , requestBody, responseHeaders, responseBody ) ); 
}

TEST_F( TestCProcessorRegistry , Basic_CaseB_allGood_returnsFalse )
{
  EXPECT_CALL( *processorA, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorB, ProcessRequest( _, _, _, _, _ ) ).Times(1).WillOnce(Return(false));
  EXPECT_CALL( *processorC, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  
  std::string url( "https://user:pass@example.com:443/path/2/B");
  tHeadersMap requestHeaders;
  tHeadersMap responseHeaders;
  std::string requestBody("");
  std::string responseBody("");

  IServiceContext& processor( processorRegistryUnderTest );
  
  ASSERT_FALSE( processor.ProcessRequest(  HttpMethod::mthd_GET, url, requestHeaders , requestBody, responseHeaders, responseBody ) ); 
}

TEST_F( TestCProcessorRegistry , Basic_CaseC_allGood )
{
  EXPECT_CALL( *processorA, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorB, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorC, ProcessRequest( _, _, _, _, _ ) ).Times(1).WillOnce(Return(true));
  
  std::string url( "https://user:pass@example.com:443/path/2/C");
  tHeadersMap requestHeaders;
  tHeadersMap responseHeaders;
  std::string requestBody("");
  std::string responseBody("");

  IServiceContext& processor( processorRegistryUnderTest );
  
  ASSERT_TRUE( processor.ProcessRequest(  HttpMethod::mthd_DELETE, url, requestHeaders , requestBody, responseHeaders, responseBody ) ); 
}

TEST_F( TestCProcessorRegistry , Basic_CallURLNotMatching )
{
  EXPECT_CALL( *processorA, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorB, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  EXPECT_CALL( *processorC, ProcessRequest( _, _, _, _, _ ) ).Times(0);
  
  std::string url( "https://user:pass@example.com:443/patch/me/badly");
  std::string requestBody("");
  std::string responseBody("");
  tHeadersMap requestHeaders;
  tHeadersMap responseHeaders;

  IServiceContext& processor( processorRegistryUnderTest );
  
  ASSERT_FALSE( processor.ProcessRequest(  HttpMethod::mthd_DELETE, url, requestHeaders , requestBody, responseHeaders, responseBody ) ); 
}