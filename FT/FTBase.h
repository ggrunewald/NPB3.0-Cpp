#ifndef FTBASE_H
#define FTBASE_H

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

//class FT;

#include "../aux2/Timer.h"
#include "../aux2/Random.h"

#define REAL 0
#define IMAG 1

#define pi 3.14159265358979323846
#define alpha 0.000001

#define fftblock_default 4*4096 //Size of L1 cache on SGI O2K

class FT;

using namespace std;

class FTBase
{
	protected:
		string BMName;

		char execClass;

		Timer * timer;

		int numThreads;

		int nx, ny, nz, maxdim, niter_default;

		int fftblock;;

		FT * master;

  //complex arrays
		double * scr;
		double * plane;
		int isize2;
		int isize3, jsize3, ksize3;
		int isize4, jsize4, ksize4;

		double * checksum;
		double * xtr;
		double * xnt;
		double * exp1, * exp2, * exp3;

		bool timeron;

		void InitialConditions(double * u0, int d1, int d2, int d3);

		void CompExp(int n, double * exponent);

		int Ilog2(int n);

		void Swarztrauber(int is,int m,int len,int n,double * x, int xoffst, int xd1,double * exponent,double * scr);

		void SetInterval(int threads, int problem_size, int * interval);
		void SetPartition(int start, int * interval, int ** prt, int numThreads);

	public:
		FTBase();
		FTBase(char c, int n, bool s);
};

#endif