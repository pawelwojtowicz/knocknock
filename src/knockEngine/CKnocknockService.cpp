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

const tKeyValueMap CKnocknockService::Login(const std::string& userId, const std::string& password)
{
  return {};
}

const tKeyValueMap CKnocknockService::Authenticate(const std::string& sessionId, const tKeyValueMap& authenticationPayload)
{
  return {};
}

const tKeyValueMap CKnocknockService::Logout(const std::string& sessionId)
{
  return {};
}

const tKeyValueMap CKnocknockService::Touch(const std::string& sessionId)
{
  return {};
}

}