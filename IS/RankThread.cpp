/*
 * RankThread.cpp
 *
 *  Created on: 19 de mar de 2016
 *      Author: guilherme
 */

#include "RankThread.h"

int RankThread::iteration = 0;

RankThread::RankThread()
{
	done = true;
	iteration = 0;
}

RankThread::~RankThread()
{
	// TODO Auto-generated destructor stub
}

RankThread::RankThread(IS * is, int Id, int s1, int e1, int s2, int e2)
{
	done = true;
	iteration = 0;
	Init(is);
    master = is;
    id=Id;
    _start = s1;
    end = e1;
    rstart = s2;
    rend = e2;
    local_hist = new int[MAX_KEY];
    state = 0;

    //ESSA PARTE EU RETIREI POIS ERA ESPECÍFICA DA SUPERCLASSE THREAD
    //DE JAVA E NÃO SABIA O QUE FAZER A RESPEITO EM C++.
    //SetPriority(Thread.MAX_PRIORITY);
    //SetDaemon(true);
}

void RankThread::Init(IS * is)
{
    //initialize shared data
	numThreads = is->GetNumThreads();

    MAX_KEY=is->MAX_KEY;

    key_array=is->key_array;

    for(int i = 0; i < 5; i++)
    	test_index_array[i]=is->test_index_array[i];

    master_hist=is->master_hist;

    partial_verify_vals=is->partial_verify_vals;
 }

bool RankThread::isDone()
{
	return done;
}

void RankThread::SetDone(bool d)
{
	done = d;
}

void RankThread::SetIteration(int i)
{
	iteration = i;
}

void RankThread::start()
{
	workingThread = new thread(&RankThread::run, this);
}

void RankThread::run()
{
	//cout << "Thread " << id << " running." << endl;

	for(;;)
	{
		//Deadlock safe!
		if(done == true)
		{
			while(done == true)
			{
				//WAIT
			}
		}

		switch(state)
		{
			case 0:
				step1();
				state = 1;
				break;
			case 1:
				step2();
				state = 0;
				break;
		}

		done = true;
	}
}

void RankThread::step1()
{
	key_array[iteration] = iteration;
	key_array[iteration+MAX_ITERATIONS] = MAX_KEY - iteration;

	for(int i=0; i<TEST_ARRAY_SIZE; i++ )
	{
		partial_verify_vals[i] = key_array[test_index_array[i]];
	}

	for(int i=0;i<MAX_KEY;i++) local_hist[i]=0;
	for(int i=_start; i<=end; i++ ) local_hist[key_array[i]]++;
	for(int i=0; i<MAX_KEY-1; i++ ) local_hist[i+1] += local_hist[i];
}

void RankThread::step2()
{
    //Parallel calculation of the master's histogram
    for(int i=rstart;i<=rend;i++)
    	for(int j=0;j<numThreads;j++)
    		master_hist[i]+=rankthreads[j]->local_hist[i];
}

void RankThread::join()
{
	workingThread->join();
}
