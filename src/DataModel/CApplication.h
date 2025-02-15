#pragma once
#include <string>
#include <vector>

namespace knocknock
{
class CApplication
{
  CApplication() = default;
  CApplication( const int id, const std::string& name, const std::string& datePublisher, const std::string& accessTOken);
  ~CApplication() = default;

  void SetName( const std::string& name ) {
    m_name = name;
  }

  void SetDataPublisher( const std::string& dataPublisher) {
    m_dataPublisher = dataPublisher;
  }

  void SetAccessToken( const std::string& accessToken ) {
    m_accessToken = accessToken;
  }

  int GetAppId() const {
    return m_appId;
  }

  const std::string& GetName() const {
    return m_name;
  }

  const std::string& GetDataPublisher() const {
    return m_dataPublisher;
  }

  const std::string& GetAccessToken() const {
    return m_accessToken;
  }

private:
  int m_appId = {0};
  std::string m_name = {} ;
  std::string m_dataPublisher = {};
  std::string m_accessToken = {};
};

using tApplications = std::vector<CApplication>;
}