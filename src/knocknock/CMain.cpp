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

bool CMain::InitModule(int argc, char** argv)
{
	for (int i = 0 ; i < argc ; ++i )
	{
		m_arguments.push_back(argv[i]);
	}
	return Initialize();
}

int CMain::GetArgumentCount()
{
	return m_arguments.size();
}

const std::string& CMain::GetArgument( const int& argNo )
{
	if ( argNo >= GetArgumentCount() )
	{
		static const std::string emptyString{};
		return emptyString;
	}
	return m_arguments.at( argNo );
}

bool CMain::Initialize()
{
	if ( GetArgumentCount() < 2 )
	{
		LOG( ERROR, "No configuration file provided. Please provide configuration file as first argument." );
		return false;
	}

	const std::string& configFileName = GetArgument(1);
	if ( !m_configuration.LoadConfig(configFileName) )
	{
		LOG( ERROR, "Failed to load configuration file: %s", configFileName.c_str() );
		return false;
	}
	LOG( INFO, "Configuration file %s loaded successfully", configFileName.c_str() );

	return true;
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