/*
 * Timer.h
 *
 *  Created on: 14 de mar de 2016
 *      Author: guilherme
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <chrono>

#define max_counters 64

using namespace std;

class Timer
{
	private:
		std::chrono::time_point<std::chrono::system_clock> start_time[max_counters];
		std::chrono::duration<double> elapsed_time[max_counters];
		std::chrono::duration<double> total_time[max_counters];

		std::chrono::duration<double> zeroTime;

	public:
		Timer();
		virtual ~Timer();

		void Start(int n);
		void Stop(int n);

		std::chrono::duration<double> ReadTimer(int n);

		void ResetAllTimers();
		void ResetTimer(int n);
};

#endif /* TIMER_H_ */
