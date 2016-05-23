/*
 * ISBase.h
 *
 *  Created on: 12 de mar de 2016
 *      Author: guilherme
 */

#ifndef ISBASE_H_
#define ISBASE_H_

#include <thread>
#include <vector>

#include "../aux2/Timer.h"
#include "../aux2/Random.h"

class RankThread;		//GAMBIARRA SUGERIDA PELO STACKOVERFLOW

class ISBase
{
	protected:
		string BMName;
		char execClass;
		int numThreads;
		Timer * timer;

		ISBase * master;

	public:
		ISBase();
		ISBase(char c, int n, bool s);

		virtual ~ISBase();

		int test_index_array[5], test_rank_array[5];

		const int  MAX_ITERATIONS=10,
				   TEST_ARRAY_SIZE=5;

		int
	       S_test_index_array[5] = {48427,17148,23627,62548,4431},
	       S_test_rank_array[5] = {0,18,346,64917,65463},

	       W_test_index_array[5] = {357773,934767,875723,898999,404505},
	       W_test_rank_array[5] = {1249,11698,1039987,1043896,1048018},

	       A_test_index_array[5] = {2112377,662041,5336171,3642833,4250760},
	       A_test_rank_array[5] = {104,17523,123928,8288932,8388264},

	       B_test_index_array[5] = {41869,812306,5102857,18232239,26860214},
	       B_test_rank_array[5] = {33422937,10244,59149,33135281,99},

	       C_test_index_array[5] = {44172927,72999161,74326391,129606274,21736814},
	       C_test_rank_array[5] = {61147,882988,266290,133997595,133525895};

		int TOTAL_KEYS;
		int MAX_KEY;
		int NUM_BUCKETS;
		int NUM_KEYS;
		int SIZE_OF_BUFFERS;

		int TOTAL_KEYS_LOG_2;
		int MAX_KEY_LOG_2;
		int NUM_BUCKETS_LOG_2;

		int passed_verification;

		int * master_hist,
			* key_array,
			* partial_verify_vals;

		int GetNumThreads();

		vector<RankThread *> rankthreads;
};

#endif /* ISBASE_H_ */
