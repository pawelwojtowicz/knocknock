#include "CMain.h"
#include "Logger.h"

namespace knocknock
{
CMain globalMain;

CMain::CMain()
: m_arguments()
, m_logger( ALL, Logger::CSimpleLogger::eComplete )
{
	__pExecutable = this;
}

void CMain::InitModule(int argc, char** argv)
{
	for (int i = 0 ; i < argc ; ++i )
	{
		m_arguments.push_back(argv[i]);
	}
	Initialize();
}

int CMain::GetArgumentCount()
{
	return m_arguments.size();
}

void CMain::Initialize()
{
  LOG( INFO, "CMain::Initialize() called %d", GetArgumentCount() );
}


int CMain::Run()
{
  LOG( INFO, "CMain::Run() called" );

  return 0;
}

void CMain::Shutdown()
{
  LOG( INFO, "CMain::Shutdown() called" );
}

}