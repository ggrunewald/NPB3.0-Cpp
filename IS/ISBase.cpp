/*
 * ISBase.cpp
 *
 *  Created on: 12 de mar de 2016
 *      Author: guilherme
 */

#include "ISBase.h"

#include <thread>
#include <iostream>

using namespace std;

ISBase::ISBase()
{
	BMName = "IS";
}

ISBase::ISBase(char c, int n, bool s)
{
	ISBase();
	execClass = c;
	numThreads = n;

	switch(execClass)
	{
		case 'S':
			for(int i = 0; i < 5; i++)
				test_index_array[i] = S_test_index_array[i];
			for(int i = 0; i < 5; i++)
				test_rank_array[i] = S_test_rank_array[i];
			TOTAL_KEYS_LOG_2 = 16;
			MAX_KEY_LOG_2 = 11;
			NUM_BUCKETS_LOG_2 = 9;
			break;
		case 'W':
			for(int i = 0; i < 5; i++)
				test_index_array[i] = W_test_index_array[i];
			for(int i = 0; i < 5; i++)
				test_rank_array[i] = W_test_rank_array[i];
			TOTAL_KEYS_LOG_2 = 20;
			MAX_KEY_LOG_2 = 16;
			NUM_BUCKETS_LOG_2 = 10;
		  break;
		case 'A':
			for(int i = 0; i < 5; i++)
				test_index_array[i] = A_test_index_array[i];
			for(int i = 0; i < 5; i++)
				test_rank_array[i] = A_test_rank_array[i];
		    TOTAL_KEYS_LOG_2  =  23;
		    MAX_KEY_LOG_2	=  19;
		    NUM_BUCKETS_LOG_2  = 10;
		    break;
		case 'B':
			for(int i = 0; i < 5; i++)
				test_index_array[i] = B_test_index_array[i];
			for(int i = 0; i < 5; i++)
				test_rank_array[i] = B_test_rank_array[i];
		    TOTAL_KEYS_LOG_2 = 25;
		    MAX_KEY_LOG_2 = 21;
		    NUM_BUCKETS_LOG_2 = 10;
		    break;
		case 'C':
			for(int i = 0; i < 5; i++)
				test_index_array[i] = C_test_index_array[i];
			for(int i = 0; i < 5; i++)
				test_rank_array[i] = C_test_rank_array[i];
		    TOTAL_KEYS_LOG_2 = 27;
		    MAX_KEY_LOG_2 = 23;
		    NUM_BUCKETS_LOG_2 = 10;
		    break;
	}

	//common variables
	TOTAL_KEYS       = (1 << TOTAL_KEYS_LOG_2);
	MAX_KEY	     = (1 << MAX_KEY_LOG_2);
	NUM_BUCKETS      = (1 << NUM_BUCKETS_LOG_2);
	NUM_KEYS	     = TOTAL_KEYS;
	SIZE_OF_BUFFERS  = NUM_KEYS;

	key_array = new int[SIZE_OF_BUFFERS];
	master_hist = new int[MAX_KEY];
	partial_verify_vals =  new int[TEST_ARRAY_SIZE];

	for( int i=0; i<MAX_KEY; i++ )
		master_hist[i] = 0;
}

ISBase::~ISBase()
{

}

int ISBase::GetNumThreads()
{
	return numThreads;
}
