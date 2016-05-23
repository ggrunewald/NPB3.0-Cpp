#ifndef CGBASE_H
#define CGBASE_H

#define cgitmax 25

#include <string>
#include <vector>
#include <iostream>

#include "CGBase.h"

class CG;

#include "../aux2/Timer.h"

using namespace std;

class CGBase
{
	protected:
		string BMName;
		char execClass;

		int na, nonzer, niter;
		double shift,rcond,zeta_verify_value;
		int nz, naa, nzz;
		int firstrow, lastrow, firstcol, lastcol;

		int * colidx, * rowstr, * iv, * arow, * acol;
		double * v, * aelt, * a, * x, * z, * p, * q, * r;
		string * t_names;
		Timer * timer;
		double * dmaster, * rhomaster, * rnormmaster;

		double alpha,beta;

		double timer_read;
		bool timeron;
		int t_init, t_bench, t_conj_grad, t_last;

		CG * master;
		int numThreads;

	public:
		CGBase();
		CGBase(char c, int n, bool s);

		double * GetDmaster();
		double * GetRhomaster();
		double * GetRnormmaster();
		int * GetColidx();
		int * GetRowstr();
		double * GetA();
		double * GetP();
		double * GetQ();
		double * GetR();
		double * GetX();
		double * GetZ();
};

#endif