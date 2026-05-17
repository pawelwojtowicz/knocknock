#pragma once
#include <string>
#include <vector>
#include <atomic>
#include <CConfiguration.h>
#include <CSQLiteDriver.h>
#include <CDatabase.h>
#include <CSessionManager.h>
#include <CSimpleLogger.h>
#include "CHTTPChannel.h"

namespace knocknock
{

class CMain
{
public:
	CMain();
	virtual ~CMain() = default;

	bool InitModule(int argc, char** argv);

	bool Initialize();
	int Run();
	void Shutdown();

	void RequestStop();

protected:
	const std::string& GetArgument( const int& argNo );
	int GetArgumentCount();

private:
	std::vector<std::string> m_arguments;

	CConfiguration m_configuration;

	DBAccess::CSQLiteDriver m_sqliteDriver;
	DBAccess::CDatabase m_database;

	CSessionManager m_sessionManager;

	CHTTPChannel m_httpChannel;

	Logger::CSimpleLogger m_logger;

	std::atomic<bool> m_running{false};
};

}

extern knocknock::CMain* __pExecutable;