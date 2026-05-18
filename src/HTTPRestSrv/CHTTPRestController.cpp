#include "CHTTPRestController.h"
#include "HTTPServerConfig.h"
#include "CHTTPConnection.h"
#include <memory>
#include <vector>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/deadline_timer.hpp>



namespace HTTPServer
{

CHTTPRestController::CHTTPRestController()
: m_run(true)
, m_ioContext()
, m_workGuard(boost::asio::make_work_guard(m_ioContext))
, m_acceptor{ m_ioContext }
{

}

void CHTTPRestController::RegisterProcessor(  const HTTPServer::HttpMethod method, const std::string& urlPattern, std::shared_ptr<IRequestProcessor> processor )
{
  m_processorRegistry.RegisterRequestProcessor( method, urlPattern, processor );
}

bool CHTTPRestController::Initialize( const HTTPServerConfig& config )
{
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), config.port );
  m_acceptor.open(endpoint.protocol());
  m_acceptor.bind( endpoint );
  m_acceptor.listen();

  // Create thread pool with configured size
  m_threadPool = std::make_unique<boost::asio::thread_pool>(config.threadPoolSize);

  // Start accepting connections
  auto socket = std::make_shared<boost::asio::ip::tcp::socket>(m_ioContext);
  AcceptConnection(socket);

  // Start multiple I/O threads
  unsigned int numThreads = std::thread::hardware_concurrency();
  if (numThreads == 0) numThreads = 2;  // Fallback
  for (unsigned int i = 0; i < numThreads; ++i)
  {
    m_ioThreads.emplace_back([this]() {
      m_ioContext.run();
    });
  }

  return true;
}

void CHTTPRestController::Shutdown()
{
  m_run = false;
  m_workGuard.reset();  // Allow io_context.run() to return
  m_ioContext.stop();
  if (m_threadPool)
  {
    m_threadPool->stop();
  }

  // Join I/O threads
  for (auto& thread : m_ioThreads)
  {
    if (thread.joinable())
    {
      thread.join();
    }
  }

  // Thread pool will be joined automatically when destroyed
}


void CHTTPRestController::AcceptConnection( std::shared_ptr<boost::asio::ip::tcp::socket> socket )
{
  m_acceptor.async_accept(*socket, [this, socket] ( boost::beast::error_code ec ) {
    if ( !ec )
    {
      auto connection = std::make_shared<CHTTPConnection>( m_processorRegistry, std::move( *socket ), *m_threadPool );
      if ( connection )
      {
        connection->WaitForRequest();
      }
    }
    auto nextSocket = std::make_shared<boost::asio::ip::tcp::socket>(m_ioContext);
    AcceptConnection( nextSocket );
  });
}

}