#pragma once
#include "CProcessorRegistry.h"
#include "IServiceContext.h"
#include <thread>
#include <atomic>
#include <memory>
#include <boost/asio.hpp>

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
  void Run();
private:
  CProcessorRegistry m_processorRegistry;

  std::atomic<bool> m_run;



  boost::asio::io_context m_ioContext;
  
  boost::asio::ip::tcp::acceptor m_acceptor;

  std::unique_ptr<std::thread> m_workerThread;
};
}