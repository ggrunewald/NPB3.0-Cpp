#include <iostream>

#include "../aux2/BMArgs.h"
#include "IS.h"

using namespace std;

int main(int argc, char *argv[])
{
	int numThreads = 2;
	char execClass = 'A';
	bool isSerial = false;

	IS * is = NULL;
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

	is = new IS(execClass, numThreads, isSerial);

	args->Banner("IS", execClass, isSerial, numThreads);

	is->RunBenchMark();

	return 0;
}
