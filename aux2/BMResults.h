/*
 * BMResults.h
 *
 *  Created on: 31 de mar de 2016
 *      Author: guilherme
 */

#ifndef BMRESULTS_H_
#define BMRESULTS_H_

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std;

class BMResults
{
	private:
		string name;
	  	string MachineName;
	  	string PrLang;
	  	char clss;
	  	int n1,n2,n3,niter;
	  	std::chrono::duration<double> time;
		double acctime,wctime,mops;
	  	double tmSent, tmReceived;
	  	int RecArrSize;
	  	string optype;
	  	int numthreads;
	  	bool serial;
	  	int pid;
	  	int verified;

	  	//BufferedWriter out = NULL;

	public:
		BMResults();
		virtual ~BMResults();

		BMResults(string bname,
				  char   CLASS,
				  int    bn1,
				  int    bn2,
				  int    bn3,
				  int    bniter,
				  std::chrono::duration<double> btime,
				  double bmops,
				  string boptype,
				  int    passed_verification,
				  bool bserial,
			      int num_threads,
			      int bid);

		void PrintVerificationStatus(char clss, int verified, string BMName);

		void Print();
};

#endif /* BMRESULTS_H_ */
