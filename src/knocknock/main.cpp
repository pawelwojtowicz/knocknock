#include "CMain.h"
#include <csignal>

knocknock::CMain* __pExecutable;

static void signalHandler(int signum)
{
	if (__pExecutable)
	{
		__pExecutable->RequestStop();
	}
}

int main(int argc, char** argv )
{
	int retVal = 0;

	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	if ( 0 != __pExecutable )
	{
		if ( __pExecutable->InitModule(argc,argv) )
		{
			retVal = __pExecutable->Run();
		}
		__pExecutable->Shutdown();
	}

	return retVal;
}
