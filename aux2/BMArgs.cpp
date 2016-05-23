/*
 * BMArgs.cpp
 *
 *  Created on: 13 de mar de 2016
 *      Author: guilherme
 */

#include "BMArgs.h"

#include <sstream>
#include <iostream>
#include <string>

using namespace std;

BMArgs::BMArgs()
{
	execClass = 'A';
	numThreads = 2;
	isSerial = false;
}

BMArgs::~BMArgs()
{
	// TODO Auto-generated destructor stub
}

bool BMArgs::ParseCmdLineArgs(int argc, char *argv[])
{
	string argumentS = "-s";
	string argumentC = "-c";
	string argumentN = "-n";

	for (int i = 1; i < argc; ++i)
	{
		//cout << argv[i] << endl;

		if(argumentS.compare(argv[i]) == 0)
		{
			isSerial = true;

			//cout << "s = true" << endl;
		}

		else if(argumentC.compare(argv[i]) == 0)
		{
			istringstream ss(argv[i+1]);

			ss >> execClass;

			//cout << "c = " << execClass << endl;

			i++;
		}

		else if(argumentN.compare(argv[i]) == 0)
		{
			istringstream ss(argv[i+1]);

			if (!(ss >> numThreads))
			    cout << "Invalid number " << argv[i+1] << endl;
			//else
				//cout << "n = " << numThreads << endl;

			i++;
		}

		else
		{
			cout << " Flag error. Use ONLY the following flags:" << endl;
			cout << " -s if serial. Default: false." << endl;
			cout << " -n x where x is the number of threads. Default: 2 threads." << endl;
			cout << " -c x where x is the execution class. Default: A." << endl;

			return false;
		}
	}

	return true;
}

void BMArgs::Banner(string BMName, char c, bool s, int n)
{
	cout << (" NAS Parallel Benchmarks C++ version (NPB3_0_CPP)") << endl;

	if(s)
		cout << " Serial Version "<< BMName << "." << c << endl;
    else
    	cout << " Multithreaded Version " << BMName << "." << c << " n=" << n << endl;
}

void BMArgs::OutOfMemoryMessage()
{
    cout << "The java maximum heap size is to small to run this benchmark class" << endl;
    cout << "To allocate more memory, use the -mxn option where n is the number of bytes to be allocated" << endl;
}

char BMArgs::getClass()
{
	return execClass;
}

int BMArgs::getNumThreads()
{
	return numThreads;
}

bool BMArgs::getIsSerial()
{
	return isSerial;
}
