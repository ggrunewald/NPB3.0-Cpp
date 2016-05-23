/*
 * Random.cpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: guilherme
 */

#include "Random.h"
#include <cmath>

Random::Random()
{
    //First 9 digits of PI
    tran = 314159265.0;
}

Random::Random(double sd)
{
	Random();

	seed = sd;
}

double Random::Randlc(double x, double a)
{
    double * y, r23, r46, t23, t46, t1, t2, t3, t4, a1, a2, x1, x2, z;
    r23 = pow(0.5,23);
    r46 = pow(r23, 2);
    t23 = pow(2.0,23);
    t46 = pow(t23, 2);
//---------------------------------------------------------------------
//   Break A into two parts such that A = 2^23 * A1 + A2.
//---------------------------------------------------------------------
    t1 = r23 * a;
    a1 = (int) t1;
    a2 = a - t23 * a1;
//---------------------------------------------------------------------
//   Break X into two parts such that X = 2^23 * X1 + X2, compute
//   Z = A1 * X2 + A2 * X1  (mod 2^23), and then
//   X = 2^23 * Z + A2 * X2  (mod 2^46).
//---------------------------------------------------------------------
    t1 = r23 * x;
    x1 = (int) t1;
    x2 = x - t23 * x1;
    t1 = a1 * x2 + a2 * x1;
    t2 = (int) (r23 * t1);
    z = t1 - t23 * t2;
    t3 = t23 * z + a2 * x2;
    t4 = (int) (r46 * t3);
    x = t3 - t46 * t4;
    return x;
}

double Random::Randlc(double a)
{
    double * y, r23, r46, t23, t46, t1, t2, t3, t4, a1, a2, x1, x2, z;
    r23 = pow(0.5,23);
    r46 = pow(r23, 2);
    t23 = pow(2.0,23);
    t46 = pow(t23, 2);
//---------------------------------------------------------------------
//   Break A into two parts such that A = 2^23 * A1 + A2.
//---------------------------------------------------------------------
    t1 = r23 * a;
    a1 = (int) t1;
    a2 = a - t23 * a1;
//---------------------------------------------------------------------
//   Break X into two parts such that X = 2^23 * X1 + X2, compute
//   Z = A1 * X2 + A2 * X1  (mod 2^23), and then
//   X = 2^23 * Z + A2 * X2  (mod 2^46).
//---------------------------------------------------------------------
    t1 = r23 * tran;
    x1 = (int) t1;
    x2 = tran - t23 * x1;
    t1 = a1 * x2 + a2 * x1;
    t2 = (int) (r23 * t1);
    z = t1 - t23 * t2;
    t3 = t23 * z + a2 * x2;
    t4 = (int) (r46 * t3);
    tran = t3 - t46 * t4;
    return(r46 * tran);
}

double Random::Vranlc(double n, double x, double a, double * y,int offset)
{
    long Lx = (long)x;
    long La = (long)a;

    for(int i=0;i<n;i++)
	{
		Lx   = (Lx*La) & (i246m1);
		y[offset+i] = (double)(d2m46*Lx);
    }
    return (double) Lx;
}

double Random::Ipow46(double a, int exponent )
{
      int n, n2;
      double q, r;
//---------------------------------------------------------------------
// Use
//   a^n = a^(n/2)*a^(n/2) if n even else
//   a^n = a*a^(n-1)       if n odd
//---------------------------------------------------------------------
      if (exponent == 0) return seed;
      q = a;
      r = 1;
      n = exponent;

      while(n>1){
         n2 = n/2;
         if (n2*2==n){
            seed = Randlc(q,q);
	    q=seed;
            n = n2;
         }else{
            seed = Randlc(r,q);
	    r=seed;
            n = n-1;
	 }
      }
      seed = Randlc(r,q);
      return seed;
  }

double Random::Power(double a, int n)
{
//c---------------------------------------------------------------------
//c     power  raises an integer, disguised as a double
//c     precision real, to an integer power
//c---------------------------------------------------------------------
      double aj,ajj,pow;
      int nj;

      pow = 1.0;
      nj = n;
      aj = a;
      while( nj != 0 ){
        if( nj%2==1 ) {
	  seed=Randlc( pow, aj );
	  pow=seed;
	}
	ajj=aj;
        seed=Randlc( aj, ajj );
	aj=seed;
        nj = nj/2;
      }
      return pow;
}

Random::~Random()
{
	// TODO Auto-generated destructor stub
}

