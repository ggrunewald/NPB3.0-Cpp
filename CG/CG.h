#ifndef CG_H
#define CG_H

#include <cmath>
#include <chrono>

#include "CGBase.h"
#include "CGWorker.h"

#include "../aux2/BMResults.h"
#include "../aux2/Random.h"

class CG : public CGBase
{
	private:
		int bid;
		BMResults results;
		bool serial;

		Random rng;

		vector<CGWorker *> worker;

	const double amult = 1220703125.0;

	void SetTimers();

	void ExecuteTask(int OrderNum);

	void Makea(int n, int nz, double * a, int * colidx, int * rowstr,
			int nonzer, double rcond, int * arow, int * acol,
			double * aelt, double * v, int * iv, double shift);

	double Conj_grad( int * colidx, int * rowstr,
				double * x, double * z, double * a,
			    	double * p, double * q, double * r,
			    	double rnorm );

	void Sprnvc(int n, int nz, double * v, int * iv, int * nzloc,
		       	int nzloc_offst, int mark[], int mark_offst);

	int Vecset(int n, double * v, int * iv,
                    		int nzv,int ival,double val);

	void Sparse(double * a, int * colidx, int * rowstr,
                     	int n, int * arow, int * acol,
		double * aelt,
		double * x, int * mark,
		int mark_offst, int * nzloc, int nzloc_offst,
		int nnza);

	int Verify(double zeta);

	void SetupThreads(CG * cg);

	double GetMFLOPS(std::chrono::duration<double> total_time, int niter);

	void PrintTimers();

	public:
		CG();
		CG(char c, int n, bool s);

		void RunBenchMark();
};

#endif