#pragma once
#include <string>
#include <vector>
#include <CConfiguration.h>
#include <CSimpleLogger.h>

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

protected:
	const std::string& GetArgument( const int& argNo );
	int GetArgumentCount();

private:
	std::vector<std::string> m_arguments;

	CConfiguration m_configuration;

  Logger::CSimpleLogger m_logger;

};

}

extern knocknock::CMain* __pExecutable;