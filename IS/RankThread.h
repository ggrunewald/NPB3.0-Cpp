/*
 * RankThread.h
 *
 *  Created on: 19 de mar de 2016
 *      Author: guilherme
 */

#ifndef RANKTHREAD_H_
#define RANKTHREAD_H_

#include "ISBase.h"
#include "IS.h"

class IS;

class RankThread : public ISBase
{
	private:
		  int id, state;
		  int _start, end, rstart, rend;
		  thread * workingThread;

		  bool done;

	protected:
		  int * local_hist;
		  void step1();
		  void step2();

	public:
		RankThread();

		RankThread(IS * is, int Id, int s1, int e1, int s2, int e2);

		virtual ~RankThread();

		int main();

		void Init(IS * is);

		void start();
		void run();
		void join();

		bool isDone();
		void SetDone(bool d);

		void SetIteration(int i);
		
		static int iteration;
};

#endif /* RANKTHREAD_H_ */
