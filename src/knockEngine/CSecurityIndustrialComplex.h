#pragma once
#include <TBuilderFactory.h>
#include <IAuthenticationMethod.h>
#include <ISessionPublisher.h>


namespace knocknock
{
class CSecurityIndustrialComplex
{
public:
  using tAuthMethodFactory  = TBuilderFactory<IAuthenticationMethod>;
  using tSessionPublisherFactory = TBuilderFactory<ISessionPublisher>;

  CSecurityIndustrialComplex();
  virtual ~CSecurityIndustrialComplex() = default;

  const tAuthMethodFactory& GetAuthMethodFactory() const { return m_authMethodFactory; };

  const tSessionPublisherFactory& GetSessionPublisherFactory() const { return m_sessionPublisherFactory; };
private:
  tAuthMethodFactory m_authMethodFactory;

  tSessionPublisherFactory m_sessionPublisherFactory;
};
}