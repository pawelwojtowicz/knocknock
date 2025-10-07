#include "CHTTPRestController.h"
#include "HTTPServerConfig.h"
#include "CHTTPConnection.h"
#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <iostream>



namespace HTTPServer
{

CHTTPRestController::CHTTPRestController()
: m_run(true)
, m_ioContext()
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

  m_workerThread = std::make_unique<std::thread>( [&] { 
    Run();
  });

  return true;
}

void CHTTPRestController::Shutdown()
{
  m_run = false;
  m_ioContext.stop();

  if ( m_workerThread )
  {
    m_workerThread->join();
  }
  
}


void CHTTPRestController::AcceptConnection( boost::asio::ip::tcp::socket& socket, boost::system::error_code ec)
{
  m_acceptor.async_accept(socket, [&] ( boost::beast::error_code ec ) {
    if ( !ec )
    {
      auto connection = std::make_shared<CHTTPConnection>( m_processorRegistry, std::move( socket ) );
      if ( connection )
      {
        connection->WaitForRequest();
      }
    }
    AcceptConnection( socket, ec);
  });

}

void CHTTPRestController::Run()
{
  while( m_run )
  {
    boost::asio::ip::tcp::socket socket(m_ioContext );
    boost::system::error_code ec;

    AcceptConnection( socket, ec );

    m_ioContext.run();
  }
}

}