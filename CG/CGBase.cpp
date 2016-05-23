#include "CGBase.h"

CGBase::CGBase()
{
    BMName = "CG";
    execClass = 'S';
    t_init = 1;
    t_bench = 2;
    t_conj_grad = 3;
    t_last = 3;
}

CGBase::CGBase(char c, int n, bool s)
{
    CGBase();

    execClass = c;
    numThreads = n;

    switch(execClass)
    {
        case 'S':
    	na=1400;
    	nonzer=7;
    	shift=10;
    	niter=15;
    	rcond=.1;
    	zeta_verify_value = 8.5971775078648;
    	break;
        case 'W':
    	na=7000;
    	nonzer=8;
    	shift=12;
    	niter=15;
    	rcond=.1;
    	zeta_verify_value = 10.362595087124;
    	break;
        case 'A':
    	na=14000;
    	nonzer=11;
    	shift=20;
    	niter=15;
    	rcond=.1;
    	zeta_verify_value = 17.130235054029;
    	break;
        case 'B':
    	na=75000;
    	nonzer=13;
    	shift=60;
    	niter=75;
    	rcond=.1;
    	zeta_verify_value = 22.712745482631;
    	break;
        case 'C':
    	na=150000;
    	nonzer=15;
    	shift=110;
    	niter=75;
    	rcond=.1;
    	zeta_verify_value = 28.973605592845;
    	break;
    }

    t_names = new string[t_last+1];

    timer = new Timer();

    nz = (na*(nonzer+1)*(nonzer+1)+ na*(nonzer+2) );
    colidx = new int[nz +1];
    rowstr = new int[na+2];
    iv = new int[2*na+2];
    arow = new int[nz+1];
    acol = new int[nz+1];
    v = new double[na+2];
    aelt = new double[nz+1];
    a = new double[nz+1];
    p = new double[na+3];
    q = new double[na+3];
    r = new double[na+3];
    x = new double[na+3];
    z = new double[na+3];
}

double * CGBase::GetDmaster()
{
    return dmaster;
}

double * CGBase::GetRhomaster()
{
    return rhomaster;
}

double * CGBase::GetRnormmaster()
{
    return rnormmaster;
}

int * CGBase::GetColidx()
{
    return colidx;
}

int * CGBase::GetRowstr()
{
    return rowstr;
}

double * CGBase::GetA()
{
    return a;
}

double * CGBase::GetP()
{
    return p;
}

double * CGBase::GetQ()
{
    return q;
}

double * CGBase::GetR()
{
    return r;
}

double * CGBase::GetX()
{
    return x;
}

double * CGBase::GetZ()
{
    return z;
}
