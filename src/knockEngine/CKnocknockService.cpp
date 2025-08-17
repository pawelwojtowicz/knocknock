#include "CKnocknockService.h"

namespace knocknock
{

CKnocknockService::CKnocknockService()
{

}

CKnocknockService::~CKnocknockService()
{

}

bool CKnocknockService::Initialize()
{
  return false;
}

void CKnocknockService::Shutdown()
{

}

const std::string CKnocknockService::Login(const std::string& userId, const std::string& password)
{
  return "";
}

const std::string CKnocknockService::Authenticate(const std::string& sessionId, const std::string& authenticationPayload)
{
  return "";
}

const std::string CKnocknockService::Logout(const std::string& sessionId)
{
  return "";
}

const std::string CKnocknockService::Touch(const std::string& sessionId)
{
  return "";
}

}