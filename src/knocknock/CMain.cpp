#include "CMain.h"
#include "Logger.h"
#include "KnocknockConst.h"
#include <filesystem>

namespace knocknock
{
CMain globalMain;

CMain::CMain()
: m_arguments()
, m_configuration()
, m_sqliteDriver()
, m_database(m_sqliteDriver)
, m_sessionManager( m_database,m_configuration)
, m_httpChannel( m_sessionManager )
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

	std::string dbLocation = m_configuration.GetParamString(cParamNamePrimaryDBLocation, cDefaultDBLocation);
	if ( !std::filesystem::exists(dbLocation) )
	{
		LOG( WARNING, "Database file %s does not exist. Scanning secondary location...", dbLocation.c_str() );
		std::string secondaryDBLocation = m_configuration.GetParamString(cParamNameSecondaryDBLocation, cDefaultDBLocation);
		if ( std::filesystem::exists(secondaryDBLocation) )
		{
			LOG( INFO, "Database file found in secondary location: %s", secondaryDBLocation.c_str() );
			dbLocation = secondaryDBLocation;
		}
		else
		{
			LOG( WARNING, "Database file %s not found in secondary location", dbLocation.c_str() );
		  LOG( WARNING, "Creating empty DB in primary location: %s", dbLocation.c_str() );

		}
	}
	m_database.OpenDatabase( dbLocation );

	m_sessionManager.Initialize();

	m_httpChannel.Initialize( m_configuration );
	return true;
}


int CMain::Run()
{
  LOG( INFO, "CMain::Run() called" );

	while (1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_sessionManager.Tick();
	}


  return 0;
}

void CMain::Shutdown()
{
	m_httpChannel.Shutdown();

	m_sessionManager.Shutdown();
  LOG( INFO, "CMain::Shutdown() called" );
}

}