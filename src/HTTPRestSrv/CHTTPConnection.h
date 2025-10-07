#pragma once
#include "IServiceContext.h"
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <chrono>

namespace HTTPServer
{
class CHTTPConnection final : public std::enable_shared_from_this<CHTTPConnection>
{
public:
  CHTTPConnection( IServiceContext& serviceContext, boost::asio::ip::tcp::socket socket);
  ~CHTTPConnection() = default;

  void WaitForRequest();

private:
  void ProcessRequest();
  void WriteResponse();
  void HandleInvalid();

  void CheckDeadline();

private:
  IServiceContext& m_serviceContext;

  boost::asio::ip::tcp::socket m_socket;
  std::string m_remoteAddress;

  boost::beast::flat_buffer m_buffer { 8192 };
  boost::beast::http::request<boost::beast::http::string_body> m_request;
  boost::beast::http::response<boost::beast::http::string_body> m_response;

  boost::asio::basic_waitable_timer<std::chrono::steady_clock> m_deadlineTimer;
};
}