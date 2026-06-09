#include <gtest/gtest.h>
#include <CSessionPublisher.h>
#include <CConfiguration.h>

#include <chrono>
#include <thread>


using namespace knocknock;

TEST(CSessionPublisherTests, InitializeAndShutdown)
{
  CConfiguration config;
  CSessionPublisher publisher;

  tSessionList sessions;

  EXPECT_TRUE(publisher.Initialize(config));

  publisher.PublishSessions(sessions);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for the publisher to process the data

  publisher.PublishSessions(sessions);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Wait for the publisher to process the data

  publisher.Shutdown();
}