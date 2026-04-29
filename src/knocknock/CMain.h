#pragma once
#include <string>
#include <vector>
#include <CSimpleLogger.h>

namespace knocknock
{

class CMain
{
public:
	CMain();
	virtual ~CMain() = default;

	void InitModule(int argc, char** argv);

	virtual void Initialize();
	virtual int Run();
	virtual void Shutdown();

protected:
	const std::string& GetArgument( const int& argNo );
	int GetArgumentCount();

private:
	std::vector<std::string> m_arguments;

  Logger::CSimpleLogger m_logger;

};

}

extern knocknock::CMain* __pExecutable;