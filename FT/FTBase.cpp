#include "FTBase.h"

FTBase::FTBase()
{
	BMName = "FT";

	timer = new Timer();

	timeron = false;

	execClass = 'S';
}

FTBase::FTBase(char c, int n, bool s)
{
	BMName = "FT";

	timer = new Timer();

	timeron = false;

	execClass = c;

            numThreads=n;

	/////////////////////////////////
	switch (execClass)
	{
		case 'S':
		  nx=ny=nz=64;
		  niter_default=6;
		  break;
		case 'W':
		  nx=ny=128;
		  nz=32;
		  niter_default=6;
		  break;
		case 'A':
		  nx=256;
		  ny=256;
		  nz=128;
		  niter_default=6;
		  break;
		case 'B':
		  nx=512;
		  ny=nz=256;
		  niter_default=20;
		  break;
		case 'C':
		  nx=ny=nz=512;
		  niter_default=20;
		  break;
    }

    maxdim = max( nx , max( ny , nx ) );

	if(s)
	{
		scr = new double[2*(maxdim+1)*maxdim];
		plane = new double[2*(maxdim+1)*maxdim];
    }

    isize2=2;
    isize3=2;
    jsize3=2*(ny+1);
    ksize3=2*(ny+1)*nx;
    isize4=2;
    jsize4=2*(ny+1);
    ksize4=2*(ny+1)*nz;

    //complex values
    checksum = new double[2*niter_default]; //isize2=2;

    xtr = new double[2*(ny+1)*nx*nz];
    xnt = new double[2*(ny+1)*nz*nx];
    exp1 = new double[2*nx];
    exp2 = new double[2*ny];
    exp3 = new double[2*nz];
}

void FTBase::InitialConditions(double * u0, int d1, int d2, int d3)
{
    double * tmp = new double[2*maxdim];

    double * RanStarts = new double[maxdim];
    			//seed has to be init here since
    			//is called 2 times

    double seed = 314159265, a = pow(5.0,13);

    double start = seed;
//---------------------------------------------------------------------
// Jump to the starting element for our first plane.
//---------------------------------------------------------------------

	Random * rng = new Random(seed);

    double an = rng->Ipow46(a, 0);

	rng->Randlc(seed, an);

	an = rng->Ipow46(a, 2*d1*d2);

//---------------------------------------------------------------------
// Go through by z planes filling in one square at a time.
//---------------------------------------------------------------------
    RanStarts[0] = start;

	for(int k=1;k<d3;k++)
	{
      seed = rng->Randlc(start, an);
      RanStarts[k]=start=seed;
    }

	for(int k=0;k<d3;k++)
	{
      double x0 = RanStarts[k];

	  for(int j=0;j<d1;j++)
	  {
        x0 = rng->Vranlc(2*d2, x0, a, tmp,0);

        for(int i=0;i<d2;i++)
		{
          u0[REAL+j*isize3+i*jsize3+k*ksize3]=tmp[REAL+i*2];
          u0[IMAG+j*isize3+i*jsize3+k*ksize3]=tmp[IMAG+i*2];
        }
      }
    }
}

void FTBase::CompExp(int n, double * exponent)
{
    int nu = n;
    int m = Ilog2(n);
    exponent[0] = m;

    double eps=1.0E-16;
    int ku = 1;
    int ln = 1;

    for(int j=1;j<=m;j++)
	{
      double t = pi / ln;
      for(int i=0;i<=ln-1;i++)
	  {
		double ti = i * t;
		int idx=(i+ku)*2;

		exponent[REAL+idx] = cos(ti);
		exponent[IMAG+idx] = sin(ti);

		if(abs(exponent[REAL+idx]) < eps)
			exponent[REAL+idx]=0;

		if(abs(exponent[IMAG+idx]) < eps)
			exponent[IMAG+idx]=0;
      }

      ku = ku + ln;

      ln = 2 * ln;
    }
}

int FTBase::Ilog2(int n)
{
    int nn, lg;

	if (n == 1)
		return 0;

	lg = 1;
    nn = 2;

	while (nn < n)
	{
      nn = nn*2;
      lg = lg+1;
    }

   return lg;
}

void FTBase::Swarztrauber(int is,int m,int len,int n,double * x, int xoffst, int xd1,double * exponent,double * scr)
{
    int i,j=0,l,mx;
    int k, n1,li,lj,lk,ku,i11,i12,i21,i22;
    int BlockStart,BlockEnd;
    int isize1=2,jsize1=2*(xd1+1);

    //complex values
    double * u1 = new double[2], * x11 = new double[2], * x21 = new double[2];

	if(timeron)
		timer->Start(4);

//---------------------------------------------------------------------
//   Perform one variant of the Stockham FFT.
//---------------------------------------------------------------------

    fftblock = fftblock_default/n;

    if (fftblock<8)
		fftblock=8;

    for(BlockStart=0; BlockStart<len;BlockStart+=fftblock)
	{
      BlockEnd = BlockStart + fftblock - 1;

	  if ( BlockEnd >= len)
		BlockEnd = len-1;

	  for(l=1;l<=m;l+=2)
	  {
		n1 = n / 2;
		lk = (int)pow(2, l-1);
		li = (int)pow(2, m-l);
		lj = 2 * lk;
		ku = li;

		for(i=0;i<=li-1;i++)
		{
			  i11 = i * lk;
			  i12 = i11 + n1;
			  i21 = i * lj;
			  i22 = i21 + lk;

			u1[REAL]= exponent[REAL + (ku+i)*2];

			if (is >= 1)
			{
				u1[IMAG]= exponent[IMAG + (ku+i)*2];
			}
			else
			{
				u1[IMAG] = - exponent[IMAG + (ku+i)*2];
			}

			for(k=0;k<=lk-1;k++)
			{
				for(j=BlockStart;j<=BlockEnd;j++)
				{
				  x11[REAL] = x[REAL+j*2+(i11+k)*jsize1 + xoffst];
				  x11[IMAG] = x[IMAG+j*2+(i11+k)*jsize1 + xoffst];
				  x21[REAL] = x[REAL+j*2+(i12+k)*jsize1 + xoffst];
				  x21[IMAG] = x[IMAG+j*2+(i12+k)*jsize1 + xoffst];
				  scr[REAL+j*isize1+(i21+k)*jsize1] = x11[REAL] + x21[REAL];
				  scr[IMAG+j*isize1+(i21+k)*jsize1] = x11[IMAG] + x21[IMAG];
				  scr[REAL+j*2+(i22+k)*jsize1] = u1[REAL] * (x11[REAL] - x21[REAL])
											   - u1[IMAG] * (x11[IMAG] - x21[IMAG]);
				  scr[IMAG+j*2+(i22+k)*jsize1] = u1[IMAG] * (x11[REAL] - x21[REAL])
											   + u1[REAL] * (x11[IMAG] - x21[IMAG]);
				}
			}
		}

		if(l==m)
		{
		  for(k=0;k<n;k++)
		  {
			for(j=BlockStart;j<=BlockEnd;j++)
			{
			  x[REAL+j*2+k*jsize1 + xoffst] = scr[REAL+j*isize1+k*jsize1];
			  x[IMAG+j*2+k*jsize1 + xoffst] = scr[IMAG+j*isize1+k*jsize1];
			}
		  }
		}
		else
		{
		  n1 = n / 2;
		  lk = (int)pow(2,l);
		  li = (int)pow(2, m - l - 1);
		  lj = 2 * lk;
		  ku = li;

		  for(i=0;i<=li-1;i++)
		  {
			i11 = i * lk;
			i12 = i11 + n1;
			i21 = i * lj;
			i22 = i21 + lk;

			u1[REAL] = exponent[REAL+(ku+i)*2];

			if (is>=1)
			{
			  u1[IMAG] = exponent[IMAG+(ku+i)*2];
			}
			else
			{
			  u1[IMAG] = - exponent[IMAG +(ku+i)*2];
			}

			for(k=0;k<=lk-1;k++)
			{
			  for(j=BlockStart;j<=BlockEnd;j++)
			  {
				x11[REAL] = scr[REAL+j*isize1+(i11+k)*jsize1];
				x11[IMAG] = scr[IMAG+j*isize1+(i11+k)*jsize1];

				x21[REAL] = scr[REAL+j*isize1+(i12+k)*jsize1];
				x21[IMAG] = scr[IMAG+j*isize1+(i12+k)*jsize1];

				x[REAL+j*2+(i21+k)*jsize1+xoffst] = x11[REAL] + x21[REAL];
				x[IMAG+j*2+(i21+k)*jsize1+xoffst] = x11[IMAG] + x21[IMAG];

				x[REAL+j*2+(i22+k)*jsize1+xoffst] = u1[REAL] * (x11[REAL] - x21[REAL])
												  - u1[IMAG] * (x11[IMAG] - x21[IMAG]);
				x[IMAG+j*2+(i22+k)*jsize1+xoffst] = u1[IMAG] * (x11[REAL] - x21[REAL])
														  + u1[REAL] * (x11[IMAG] - x21[IMAG]);
			  }
			}
		  }
		}
      }
    }

    if(timeron)
		timer->Stop(4);
}

void FTBase::SetInterval(int threads, int problem_size, int * interval)
{
    interval[0] = (int)problem_size/threads;

    for(int i=1;i<threads;i++)
        interval[i]=interval[0];

    int remainder = problem_size%threads;

    for(int i=0;i<remainder;i++)
        interval[i]++;
}

void FTBase::SetPartition(int start, int * interval, int ** prt, int numThreads)
{
    prt[0][0]=start;

    if(start==0)
        prt[0][1] = interval[0]-1;
    else
        prt[0][1]=interval[0];

    for(int i=1;i<numThreads;i++)
    {
      prt[i][0]=prt[i-1][1]+1;
      prt[i][1]=prt[i-1][1]+interval[i];
    }
}