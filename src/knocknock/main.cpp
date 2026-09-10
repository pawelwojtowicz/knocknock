#include "CMain.h"
#include <csignal>
#include <cstdio>
#include <exception>

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
		try
		{
			if ( __pExecutable->InitModule(argc,argv) )
			{
				retVal = __pExecutable->Run();
			}
		}
		catch ( const std::exception& e )
		{
			fprintf(stderr, "Fatal error during startup: %s\n", e.what());
			retVal = 1;
		}
		__pExecutable->Shutdown();
	}

	return retVal;
}
