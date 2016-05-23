#include <iostream>

#include "FT.h"

#include "../aux2/BMArgs.h"

using namespace std;

int main(int argc, char *argv[])
{

	int numThreads = 2;
	char execClass = 'A';
	bool isSerial = false;

	FT * ft = NULL;
	
	BMArgs * args = new BMArgs();

	if(argc > 1)
	{
		bool ret = args->ParseCmdLineArgs(argc, argv);

		if(!ret)
			return -1;

		numThreads = args->getNumThreads();
		execClass = args->getClass();
		isSerial = args->getIsSerial();
	}

	try
	{
		ft = new FT(execClass, numThreads, isSerial);
    }
	catch(const std::exception& e)
	{
      args->OutOfMemoryMessage();

      return -1;
    }

	args->Banner("FT", execClass, isSerial, numThreads);

	ft->RunBenchMark();
}