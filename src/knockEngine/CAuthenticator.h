#pragma once
#include <string>

namespace knocknock
{
class CSession;

class CAuthenticator
{
public:
  CAuthenticator();
  ~CAuthenticator();

  std::string Login(CSession& session, const std::string& username, const std::string& password);

  std::string Authenticate(CSession& session, const std::string& authenticationPaylod);
};
}