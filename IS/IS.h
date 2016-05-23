/*
 * IS.h
 *
 *  Created on: 14 de mar de 2016
 *      Author: guilherme
 */

#ifndef IS_H_
#define IS_H_

#include <iostream>
#include <chrono>
#include <vector>

#include "ISBase.h"
#include "RankThread.h"
#include "../aux2/BMResults.h"

#define amult 1220703125

using namespace std;

class IS: public ISBase
{
	private:
		bool isSerial;
		int bid;

		Random rng;

		void InitKeys(double a);

		void Rank(int iteration);

		void Partial_Verify(int iteration);
		int Full_Verify();

		double GetMOPS(std::chrono::duration<double> total_time,int niter,int num_keys);

		void SetupThreads(IS * is);

	public:
		IS();
		IS(char c, int n, bool s);
		virtual ~IS();

		void RunBenchMark();
		void doSort();
};

#endif /* IS_H_ */
