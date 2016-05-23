#ifndef FT_H
#define FT_H

#include <cmath>
#include <chrono>

#include "FTBase.h"

#include "../aux2/BMResults.h"
#include "../aux2/Random.h"

class FT : public FTBase
{
	private:
		int bid;

		bool serial, done;

		Random rng;

		void SetTimers();
		void PrintTimers();

		void Appft_serial();
		void Appft();

		int Verify(int ires,int n1,int n2,int n3,int nt, double * cksum);

		double GetMFLOPS(std::chrono::duration<double> total_time,int nx,int ny,int nz);

		void FftXYZ(int sign, double * x, double * exp1, double * exp2, double * exp3,int n1,int n2,int n3);

		void CalculateChecksum(double * csum, int csmffst, int iterN,double * u,int d1, int d2, int d3);

		void setupThreads();

	public:
		FT();
		FT(char c, int n, bool s);

		~FT();

		void RunBenchMark();
};

#endif