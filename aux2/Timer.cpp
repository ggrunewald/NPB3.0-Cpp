/*
 * Timer.cpp
 *
 *  Created on: 14 de mar de 2016
 *      Author: guilherme
 */

#include "Timer.h"

Timer::Timer()
{
	std::chrono::time_point<std::chrono::system_clock> testTime;

	testTime = std::chrono::system_clock::now();

	zeroTime = testTime - testTime;
}

Timer::~Timer()
{

}

void Timer::Start(int n)
{
    start_time[n] = std::chrono::system_clock::now();
}

void Timer::Stop(int n)
{
    elapsed_time[n] = std::chrono::system_clock::now() - start_time[n];
    total_time[n] += elapsed_time[n];
}

std::chrono::duration<double> Timer::ReadTimer(int n)
{
    return total_time[n];
}

void Timer::ResetTimer(int n)
{
	elapsed_time[n] = zeroTime;
	total_time[n] 	= zeroTime;
}

void Timer::ResetAllTimers()
{
    for(int i=0;i<max_counters;i++)
        ResetTimer(i);
}
