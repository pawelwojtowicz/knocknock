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

const CUserSession& CKnocknockService::Identify( const std::string& userId)
{
  return empty;

}

const CUserSession& CKnocknockService::Authenticate( const std::string& userId, const std::string& authenticationString)
{
  return empty;
}

}