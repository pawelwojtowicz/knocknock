#include "CHTTPConnection.h"
#include "HTTPServerTypes.h"
#include <iostream>

namespace HTTPServer
{

CHTTPConnection::CHTTPConnection( IServiceContext& serviceContext, boost::asio::ip::tcp::socket socket)
: m_serviceContext(serviceContext)
, m_socket( std::move( socket) )
, m_remoteAddress ( m_socket.remote_endpoint().address().to_string() )
, m_deadlineTimer( m_socket.get_executor(), std::chrono::seconds(30) )
{

}

void CHTTPConnection::WaitForRequest()
{
  auto self = shared_from_this();

  boost::beast::http::async_read( m_socket, m_buffer, m_request, [ self ] ( boost::beast::error_code ec, std::size_t ) {
    if ( !ec )
    {
      self->ProcessRequest();
    }
    else
    {

    }
  });
  CheckDeadline();
}

void CHTTPConnection::ProcessRequest()
{
  m_response.version(m_request.version() );
  m_response.keep_alive(false);

  HttpMethod method{HttpMethod::mthd_INVALID};

  switch ( m_request.method() )
  {
  case boost::beast::http::verb::get:
    method = HttpMethod::mthd_GET;
    break;
  case boost::beast::http::verb::post:
    method = HttpMethod::mthd_POST;
    break;
  case boost::beast::http::verb::put:
    method = HttpMethod::mthd_PUT;
    break;
  case boost::beast::http::verb::patch:
    method = HttpMethod::mthd_PATCH;
    break;
  case boost::beast::http::verb::delete_:
    method = HttpMethod::mthd_DELETE;
    break;
  default:;
  }

  const std::string url( m_request.target() );

  if ( !m_serviceContext.ProcessRequest(method, url, m_request.body(), m_response.body()) )
  {
    HandleInvalid(); 
  }

  m_request.body().clear();
  m_response.prepare_payload();
  WriteResponse();
}

void CHTTPConnection::WriteResponse()
{
  auto self = shared_from_this();

  m_response.set( boost::beast::http::field::content_length, std::to_string( m_response.body().size() ) );
  
  boost::beast::http::async_write( m_socket, m_response, [ self ]( boost::beast::error_code ec, std::size_t) {
    self->m_socket.shutdown( boost::asio::ip::tcp::socket::shutdown_send, ec);
    self->m_deadlineTimer.cancel();
  });

}

void CHTTPConnection::HandleInvalid()
{
  m_response.result( boost::beast::http::status::bad_request );
  m_response.set( boost::beast::http::field::content_type, "text/plain");
  m_response.body() = "Invalid request method"; 
}

void CHTTPConnection::CheckDeadline()
{
  auto self = shared_from_this();

  m_deadlineTimer.async_wait( [self] ( boost::beast::error_code ec) {
    if ( !ec)
    {
      self->m_socket.close(ec);
    }
  });

}

}