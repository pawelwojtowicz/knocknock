#pragma once
#include "CProcessorRegistry.h"
#include "IServiceContext.h"
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>

namespace HTTPServer
{
class HTTPServerConfig;

class CHTTPRestController
{
public:
  CHTTPRestController( );
  ~CHTTPRestController() = default;

  void RegisterProcessor(  const HTTPServer::HttpMethod method, const std::string& urlPattern, std::shared_ptr<IRequestProcessor> processor );

  bool Initialize( const HTTPServerConfig& config );

  void Shutdown();

private:
  void AcceptConnection( boost::asio::ip::tcp::socket& socket, boost::system::error_code ec);
private:
  CProcessorRegistry m_processorRegistry;

  std::atomic<bool> m_run;

  boost::asio::io_context m_ioContext;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_workGuard;
  
  boost::asio::ip::tcp::acceptor m_acceptor;

  std::unique_ptr<boost::asio::thread_pool> m_threadPool;

  std::vector<std::thread> m_ioThreads;
};
}