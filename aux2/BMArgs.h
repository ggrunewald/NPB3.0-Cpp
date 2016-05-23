/*
 * BMArgs.h
 *
 *  Created on: 13 de mar de 2016
 *      Author: guilherme
 */

#ifndef BMARGS_H_
#define BMARGS_H_

#include <string>

using namespace std;

class BMArgs
{
	private:
		char execClass;
		int numThreads;
		bool isSerial;

	public:
		BMArgs();
		virtual ~BMArgs();

		bool ParseCmdLineArgs(int argc, char *argv[]);

		void Banner(string BMName, char c, bool s, int n);

        void OutOfMemoryMessage();

		char getClass();
		int getNumThreads();
		bool getIsSerial();
};

#endif /* BMARGS_H_ */
