/*
 * IS.cpp
 *
 *  Created on: 14 de mar de 2016
 *      Author: guilherme
 */

#include "IS.h"

IS::IS(char c, int n, bool s) : ISBase(c, n, s)
{
	isSerial = s;

	bid = -1;
}

IS::~IS()
{

}

void IS::RunBenchMark()
{
	cout << " Size: " << TOTAL_KEYS << " Iterations: " << MAX_ITERATIONS << endl;

	//Initialize timer
	timer = new Timer();
	timer->ResetTimer(0);

	//Generate random number sequence and subsequent keys on all procs
	InitKeys(amult); 	// Random number gen seed
						// Random number gen mult

	/* Do one iteration for free (i.e., "untimed") to guarantee initialization of
		all data and code pages and respective tables */
	if(isSerial)
	{
		Rank(1);
	}
	else
	{
		SetupThreads(this);

		RankThread::iteration = 1;

		doSort();

		for(int i=0; i<MAX_KEY; i++ )
			master_hist[i] = 0;

		doSort();

		Partial_Verify(1);
	}

	/*Start verification counter */
	passed_verification = 0;

	if( execClass != 'S' )
		cout << "\n     iteration#" << endl;

	timer->Start(0);

	/*This is the main iteration */
	for(int it=1; it<=MAX_ITERATIONS; it++ )
	{
		if( execClass != 'S' ) cout << "	  " << it << endl;

		if(isSerial)
		{
			Rank(it);
		}
		else
		{
			RankThread::iteration = it;

			doSort();

			for(int i=0; i<MAX_KEY; i++ )
				master_hist[i] = 0;

			doSort();
			Partial_Verify(it);
		}
	}

	timer->Stop(0);

	/*This tests that keys are in sequence: sorting of last ranked key seq
	  occurs here, but is an untimed operation			       */
	Full_Verify();

	int verified = 0;

	if( passed_verification == 5*MAX_ITERATIONS + 1 )
		verified = 1;

	std::chrono::duration<double> tm = timer->ReadTimer(0);

	BMResults * results = new BMResults(BMName,
						execClass,
						TOTAL_KEYS,
						0,
						0,
						MAX_ITERATIONS,
						tm,
						GetMOPS(tm, MAX_ITERATIONS, TOTAL_KEYS),
						"keys ranked",
						verified,
						isSerial,
						numThreads,
						bid);

	results->PrintVerificationStatus(execClass, verified, BMName);

	results->Print();
}

double IS::GetMOPS(std::chrono::duration<double> total_time,int niter,int num_keys)
{
	double mops = 0.0;

	double tt = total_time.count();

	if( tt > 0 )
	{
		mops = (double) niter+num_keys;
		mops *= niter / (tt*1000000.0);
	}

	return mops;
}

void IS::InitKeys(double a)
{
	double x;

    int k = MAX_KEY/4;

    for (int i=0; i<NUM_KEYS; i++)
    {
    	x =  rng.Randlc(a);
		x += rng.Randlc(a);
		x += rng.Randlc(a);
		x += rng.Randlc(a);

      key_array[i] = (int)(x*k);
    }
  }

void IS::Rank(int iteration)
{
	key_array[iteration] = iteration;
	key_array[iteration + MAX_ITERATIONS] = MAX_KEY - iteration;

	for(int  i=0; i<TEST_ARRAY_SIZE; i++ )
	{
		partial_verify_vals[i] = key_array[test_index_array[i]];
	}

	/*  Clear the work array */
	for(int i=0;i<MAX_KEY;i++)
		master_hist[i] = 0;

	/*  In this section, the keys themselves are used as their
		own indexes to determine how many of each there are: their
		individual population  */

	for(int i=0;i<NUM_KEYS;i++)
	{
		master_hist[key_array[i]]++;
	}
	/* Now they have individual key   */
	/* population		      */

	/*  Density to Distribution conversion */
	for(int i=0; i<MAX_KEY-1; i++ )
	{
		master_hist[i+1] += master_hist[i];
	}

	Partial_Verify(iteration);
}

void IS::Partial_Verify(int iteration)
{
	for( int i=0; i<TEST_ARRAY_SIZE; i++ )
	{
		//test vals were put here
		int k = partial_verify_vals[i];

		int offset=iteration;

		if( 0 <= k  &&  k <= NUM_KEYS-1 )
		{
			switch(execClass)
			{
				case 'S':
					if( i <= 2 ) offset=iteration;
					else offset=-iteration;
					break;
				case 'W':
					if( i < 2 ) offset=iteration-2;
					else offset=-iteration;
					break;
				case 'A':
					if( i <= 2 ) offset=iteration-1;
					else offset=-iteration+1;
					break;
				case 'B':
					if( i == 1 || i == 2 || i == 4 ) offset=iteration;
					else offset=-iteration;
					break;
				case 'C':
					if( i <= 2 ) offset=iteration;
					else offset=-iteration;
					break;
			}

			if( master_hist[k-1] != test_rank_array[i]+offset )
			  cout << "Failed partial verification: iteration " << iteration << ", test key " << i << endl;
			else
			  passed_verification++;
		}
	}
}

int IS::Full_Verify()
{
	/*To save copy and memory sorting can be done directly:*/
	int  key=0,idx=0;
	for(int i=0; i<NUM_KEYS; i++ )
	{
		while(idx==master_hist[key])
		{
		  key++;
		  if( key>=MAX_KEY||idx>=NUM_KEYS)
			  break;
		}

		key_array[idx] = key;

		idx++;
	}

	//Confirm keys correctly sorted: count incorrectly sorted keys, if any
	int count = 0;

	for(int i=1; i<NUM_KEYS; i++ )
		if( key_array[i-1] > key_array[i] )
			count++;

	if( count != 0 )
		cout << "Full_verify: number of keys out of sort: " << count << endl;

	else
		passed_verification++;

	return passed_verification;
  }

void IS::SetupThreads(IS * is)
{
	int start=0, end=0, remainder=TOTAL_KEYS%numThreads, offset=0;
	int rstart=0, rend=0, rremainder=MAX_KEY%numThreads, roffset=0;

	for(int i=0;i<numThreads;i++)
	{
		start = i*(TOTAL_KEYS/numThreads) + offset;
		end = i*(TOTAL_KEYS/numThreads) + (TOTAL_KEYS/numThreads) - 1 + offset;

		if(remainder>0)
		{
			remainder--;
			offset++;
			end++;
		}

		rstart = i*(MAX_KEY/numThreads) + roffset;

		rend = i*(MAX_KEY/numThreads) + (MAX_KEY/numThreads) - 1 + roffset;

		if(rremainder>0)
		{
		  rremainder--;
		  roffset++;
		  rend++;
		}

		RankThread * tmpThread = new RankThread(is,i,start,end,rstart,rend);

		rankthreads.push_back(tmpThread);

		rankthreads[i]->start();
	}

	for(int i=0; i<numThreads; i++)
	{
	      rankthreads[i]->rankthreads = rankthreads;
	}
}

void IS::doSort()
{
	for(int m = 0; m < numThreads; m++)
	{
		rankthreads[m]->SetDone(false);
	}

	for(int m = 0; m < numThreads; m++)
	{
	  	while(!rankthreads[m]->isDone());
	  		//cout << "waiting " << m << endl;
	}
}
