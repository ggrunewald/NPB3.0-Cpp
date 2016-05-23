/*
 * Random.h
 *
 *  Created on: 31 de mar de 2016
 *      Author: guilherme
 */

#ifndef RANDOM_H_
#define RANDOM_H_

class Random
{
	private:
		//default seed
	  	double tran;

	  	//Random Number Multiplier
	  	const double amult = 1220703125.0;

	  	int KS = 0;
	  	double R23, R46, T23, T46;
	  	double seed;

	  	//constant
	  	const double d2m46 = 1.42109e-14;	//pow(0.5,46)

	protected:
	  	//constant
	  	const long i246m1 = 70368744177663;	//(long)pow(2,46)-1

	public:
		Random();
		Random(double sd);

		//Random number generator with an external seed
		double Randlc(double x, double a);
		//Random number generator with an internal seed
		double Randlc(double a);

		double Vranlc(double n, double x, double a, double y[],int offset);

		double Ipow46(double a, int exponent);

		double Power(double a, int n);

		virtual ~Random();
};

#endif /* RANDOM_H_ */
