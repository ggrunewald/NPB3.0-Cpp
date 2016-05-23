#include "FT.h"

#include <stdio.h>	//TESTE

FT::FT()
{
	bid=-1;
	serial = true;
	done = false;
}

FT::FT(char c, int n, bool s) : FTBase(c, n, s)
{
	bid=-1;
	done = false;
	serial = s;
            numThreads=n;
}

FT::~FT()
{

}

void FT::RunBenchMark()
{
    cout << " Size = " << nx << " X " << ny << " X " << nz
                       << " niter = " << niter_default << endl;

    SetTimers();

    timer->ResetAllTimers();

    if(serial)
	Appft_serial();
    else
	Appft();

    if(timeron)
	timer->Start(14);

	//DEU ERRO AQUI
	int verified = Verify(4, nx, ny, nz, niter_default, checksum);

	if(timeron)
		timer->Stop(14);

    timer->Stop(1);

    std::chrono::duration<double> tm = timer->ReadTimer(0);

    BMResults * results = new BMResults(BMName,
										execClass,
										nx,
										ny,
										nz,
										niter_default,
										tm,
										GetMFLOPS(tm,nx,ny,nz),
										"floating point",
										verified,
										serial,
										numThreads,
										bid);

	results->Print();

	if(timeron)
		PrintTimers();

	done = true;

}

void FT::SetTimers()
{
    FILE * fp  = fopen ("timer.flag", "w");

    if (fp != NULL)
        timeron = true;

    else
		timeron = false;
}

void FT::PrintTimers()
{
	std::chrono::duration<double> tm;
    cout << "  SECTION   Time (secs)" << endl;
	tm = timer->ReadTimer(1);
    cout << "FT time =		      " << tm.count() << endl;
	tm = timer->ReadTimer(2);
    cout << "WarmUp time =		      " << tm.count() << endl;
    tm = timer->ReadTimer(3);
	cout << "ffXYZ body time =	      " << tm.count() << endl;
    tm = timer->ReadTimer(4);
	cout << "Swarztrauber body time =      " << tm.count() << endl;
    tm = timer->ReadTimer(5);
	cout << "Redistribution time =	      " << tm.count() << endl;
    tm = timer->ReadTimer(6);
	cout << "Transposition time =	      " << tm.count() << endl;
    tm = timer->ReadTimer(7);
	cout << "X time =		      " << tm.count() << endl;
    tm = timer->ReadTimer(8);
	cout << "Y time =		      " << tm.count() << endl;
    tm = timer->ReadTimer(9);
	cout << "Z time =		      " << tm.count() << endl;
    tm = timer->ReadTimer(10);
	cout << "CalculateChecksum =	      " << tm.count() << endl;
    tm = timer->ReadTimer(11);
	cout << "evolve =		      " << tm.count() << endl;
    tm = timer->ReadTimer(12);
	cout << "compute_initial_conditions =  " << tm.count() << endl;
    tm = timer->ReadTimer(13);
	cout << "twiddle =		      " << tm.count() << endl;
    tm = timer->ReadTimer(14);
	cout << "verify =		      " << tm.count() << endl;
    tm = timer->ReadTimer(15);
	cout << "fftXYZ =		      " << tm.count() << endl;

  }

void FT::Appft_serial()
{
	if(timeron)
		timer->Start(2);

	InitialConditions(xtr,ny,nx,nz);

    CompExp( nx, exp1 );

    CompExp( ny, exp2 );

    CompExp( nz, exp3 ) ;

	FftXYZ(1, xtr, exp2, exp1, exp3,ny,nx,nz);

	if(timeron)
		timer->Stop(2);

    timer->Start(1);

    if(timeron)
		timer->Start(12);

	InitialConditions(xtr,ny,nx,nz);

    if(timeron)
		timer->Stop(12);

	if(timeron)
		timer->Start(15);

	FftXYZ(1,xtr,exp2,exp1,exp3,ny,nx,nz);

	if(timeron)
		timer->Stop(15);

    double ap =  (- 4.0 * alpha * pow(pi,2) );
    int n12 = nx/2;
    int n22 = ny/2;
    int n32 = nz/2;

    for(int it=0;it<niter_default;it++)
	{
		if(timeron)
			timer->Start(11);

		for(int i=0;i<nx;i++)
		{
			int ii = i-((i)/n12)*nx;
			int ii2 = ii*ii;
			for(int k=0;k<nz;k++)
			{
				int kk = k-((k)/n32)*nz;
				int ik2 = ii2 + kk*kk;
				for(int j=0;j<ny;j++)
				{
					int jj = j-((j)/n22)*ny;
					xnt[REAL+j*isize4+k*jsize4+i*ksize4] =
					xtr[REAL+j*isize3+i*jsize3+k*ksize3]*
					exp((ap*(jj*jj + ik2))*(it+1));

					xnt[IMAG+j*isize4+k*jsize4+i*ksize4] =
					xtr[IMAG+j*isize3+i*jsize3+k*ksize3]*
					exp((ap*(jj*jj + ik2))*(it+1));

					//cout << "xnt[REAL+j*isize4+k*jsize4+i*ksize4] = " << xnt[REAL+j*isize4+k*jsize4+i*ksize4] << endl;
					//cout << "xnt[IMAG+j*isize4+k*jsize4+i*ksize4] = " << xnt[IMAG+j*isize4+k*jsize4+i*ksize4] << endl;
				}
			}
		}

      if(timeron)
		timer->Stop(11);

      if(timeron)
		timer->Start(15);

      FftXYZ(-1,xnt,exp2,exp3,exp1,ny,nz,nx);

      if(timeron)
		timer->Stop(15);

      if(timeron)
		timer->Start(10);

      CalculateChecksum(checksum, REAL+it*isize2, it, xnt, ny, nz, nx);

	  if(timeron)
		timer->Stop(10);
    }
}

void FT::Appft()
{
    if(timeron)
        timer->Start(2);

    InitialConditions(xtr,ny,nx,nz);

    CompExp( nx, exp1 );
    CompExp( ny, exp2 );
    CompExp( nz, exp3 );

    setupThreads();
/*
    for(int m=0;m<num_threads;m++)
      synchronized(doFFT[m]){
        doFFT[m].setVariables(1,false,xtr,exp2,exp1,exp3);
    }
    doFFT();
    doFFT();
    doFFT();
    if(timeron) timer.stop(2);

    timer.start(1);
    if(timeron) timer.start(12);
    initial_conditions(xtr,ny,nx,nz);
    if(timeron) timer.stop(12);

    if(timeron) timer.start(15);
    for(int m=0;m<num_threads;m++)
      synchronized(doFFT[m]){
        doFFT[m].setVariables(1,false,xtr,exp2,exp1,exp3);
    }
    doFFT();
    doFFT();
    doFFT();
    if(timeron) timer.stop(15);

    for(int it=0;it<niter_default;it++){
      if(timeron) timer.start(11);
      doEvolve(it);
      if(timeron) timer.stop(11);

      if(timeron) timer.start(15);
      for(int m=0;m<num_threads;m++)
        synchronized(doFFT[m]){
          doFFT[m].setVariables(-1,true,xnt,exp2,exp3,exp1);
      }

      if(timeron) timer.start(3);
      if(timeron) timer.start(7);
      doFFT();
      if(timeron) timer.stop(7);

      if(timeron) timer.start(8);
      doFFT();
      if(timeron) timer.stop(8);

      if(timeron) timer.start(9);
      doFFT();
      if(timeron) timer.stop(9);
      if(timeron) timer.stop(3);
      if(timeron) timer.stop(15);

      if(timeron) timer.start(10);
      CalculateChecksum(checksum, REAL+it*isize2, it, xnt, ny, nz, nx);
      if(timeron) timer.stop(10);
    }
    */
}

int FT::Verify(int ires,int n1,int n2,int n3,int nt, double * cksum)
{
    int verified = -1;

    bool * temp = new bool[niter_default];

    double * cexpd = new double[2*21];

    if( (n1==64)&&(n2==64)&&(n3==64)&&(nt==6) )
	{
//
// Class S reference values.
//
            cexpd[REAL+0*2] = 554.610467831537;
            cexpd[REAL+1*2] = 554.640308349202;
            cexpd[REAL+2*2] = 554.616607971787;
            cexpd[REAL+3*2] = 554.544127865227;
            cexpd[REAL+4*2] = 554.427270713770;
            cexpd[REAL+5*2] = 554.270107440800;

            cexpd[IMAG+0*2] = 484.537877237905;
            cexpd[IMAG+1*2] = 486.531977345665;
            cexpd[IMAG+2*2] = 488.392628557438;
            cexpd[IMAG+3*2] = 490.128878761188;
            cexpd[IMAG+4*2] = 491.749152819072;
            cexpd[IMAG+5*2] = 493.261296332471;

         }else if ((n1 == 128) && (n2 == 128) &&
                  (n3 == 32) && (nt == 6)) {
//
// Class W reference values.
//
            cexpd[REAL+0*2] = 567.363025867117;
            cexpd[REAL+1*2] = 563.145483060159;
            cexpd[REAL+2*2] = 559.404191607951;
            cexpd[REAL+3*2] = 556.071576693173;
            cexpd[REAL+4*2] = 553.091661620263;
            cexpd[REAL+5*2] = 550.417727428295;

            cexpd[IMAG+0*2] = 529.326371681732;
            cexpd[IMAG+1*2] = 528.216681890972;
            cexpd[IMAG+2*2] = 527.101335477578;
            cexpd[IMAG+3*2] = 526.004466671071;
            cexpd[IMAG+4*2] = 524.941757355376;
            cexpd[IMAG+5*2] = 523.922894254010;
//
         }else if( (n1 == 256) && (n2 == 256) &&
                   (n3 == 128) && (nt == 6)) {
//
// Class A reference values.
//
            cexpd[REAL+0*2] = 504.675109029204;
            cexpd[REAL+1*2] = 505.942844223165;
            cexpd[REAL+2*2] = 506.939305053780;
            cexpd[REAL+3*2] = 507.790904986162;
            cexpd[REAL+4*2] = 508.524930016969;
            cexpd[REAL+5*2] = 509.150332466703;

            cexpd[IMAG+0*2] = 511.406420211119;
            cexpd[IMAG+1*2] = 509.882591447474;
            cexpd[IMAG+2*2] = 509.816028813423;
            cexpd[IMAG+3*2] = 510.135238685207;
            cexpd[IMAG+4*2] = 510.493092269075;
            cexpd[IMAG+5*2] = 510.793411986929;
//
         }else if ((n1 == 512) && (n2 == 256) &&
                  (n3 == 256) && (nt == 20)) {
//
// Class B reference values.
//
            cexpd[REAL+0*2]  = 517.766007083629;
            cexpd[REAL+1*2]  = 515.453771683778;
            cexpd[REAL+2*2]  = 514.642562837416;
            cexpd[REAL+3*2]  = 514.239514309417;
            cexpd[REAL+4*2]  = 513.964304584820;
            cexpd[REAL+5*2]  = 513.743983117218;
            cexpd[REAL+6*2]  = 513.556342198882;
            cexpd[REAL+7*2]  = 513.392728536355;
            cexpd[REAL+8*2]  = 513.248706069722;
            cexpd[REAL+9*2]  = 513.121408123512;
            cexpd[REAL+10*2] = 513.008666694153;
            cexpd[REAL+11*2] = 512.908688150414;
            cexpd[REAL+12*2] = 512.819922514643;
            cexpd[REAL+13*2] = 512.741007251216;
            cexpd[REAL+14*2] = 512.670739891043;
            cexpd[REAL+15*2] = 512.608061089712;
            cexpd[REAL+16*2] = 512.552040967780;
            cexpd[REAL+17*2] = 512.501866322912;
            cexpd[REAL+18*2] = 512.456828191936;
            cexpd[REAL+19*2] = 512.416309881155;

            cexpd[IMAG+0*2]  = 507.781963970021;
            cexpd[IMAG+1*2]  = 508.826564598980;
            cexpd[IMAG+2*2]  = 509.622515241332;
            cexpd[IMAG+3*2]  = 510.103964271548;
            cexpd[IMAG+4*2]  = 510.399287512493;
            cexpd[IMAG+5*2]  = 510.596429059181;
            cexpd[IMAG+6*2]  = 510.742044121265;
            cexpd[IMAG+7*2]  = 510.859285282714;
            cexpd[IMAG+8*2]  = 510.959356087799;
            cexpd[IMAG+9*2]  = 511.047658965145;
            cexpd[IMAG+10*2] = 511.126872149252;
            cexpd[IMAG+11*2] = 511.198436769166;
            cexpd[IMAG+12*2] = 511.263252510240;
            cexpd[IMAG+13*2] = 511.321989946146;
            cexpd[IMAG+14*2] = 511.375222369997;
            cexpd[IMAG+15*2] = 511.423475769279;
            cexpd[IMAG+16*2] = 511.467243829933;
            cexpd[IMAG+17*2] = 511.506989577152;
            cexpd[IMAG+18*2] = 511.543143136488;
            cexpd[IMAG+19*2] = 511.576099421921;
//
         }else if ((n1 == 512) && (n2 == 512) &&
                   (n3 == 512) && (nt == 20)) {
//
// Class C reference values.
//
            cexpd[REAL+0*2]  = 519.509526227300;
            cexpd[REAL+1*2]  = 515.543859957957;
            cexpd[REAL+2*2]  = 514.469441642993;
            cexpd[REAL+3*2]  = 514.016697410895;
            cexpd[REAL+4*2]  = 513.756679830484;
            cexpd[REAL+5*2]  = 513.582742267971;
            cexpd[REAL+6*2]  = 513.458570516031;
            cexpd[REAL+7*2]  = 513.366833473309;
            cexpd[REAL+8*2]  = 513.297154965605;
            cexpd[REAL+9*2]  = 513.242682685378;
            cexpd[REAL+10*2] = 513.198749539465;
            cexpd[REAL+11*2] = 513.162155826528;
            cexpd[REAL+12*2] = 513.130708574098;
            cexpd[REAL+13*2] = 513.102907097523;
            cexpd[REAL+14*2] = 513.077725805439;
            cexpd[REAL+15*2] = 513.054464504081;
            cexpd[REAL+16*2] = 513.032645619511;
            cexpd[REAL+17*2] = 513.011943789586;
            cexpd[REAL+18*2] = 512.992137646478;
            cexpd[REAL+19*2] = 512.973076763330;

            cexpd[IMAG+0*2]  = 514.903610728119;
            cexpd[IMAG+1*2]  = 512.759454171365;
            cexpd[IMAG+2*2]  = 512.226817025807;
            cexpd[IMAG+3*2]  = 512.110660806034;
            cexpd[IMAG+4*2]  = 512.116000503617;
            cexpd[IMAG+5*2]  = 512.151308490578;
            cexpd[IMAG+6*2]  = 512.188724342244;
            cexpd[IMAG+7*2]  = 512.220957287940;
            cexpd[IMAG+8*2]  = 512.247105918396;
            cexpd[IMAG+9*2]  = 512.267997365918;
            cexpd[IMAG+10*2] = 512.284720441580;
            cexpd[IMAG+11*2] = 512.298219473756;
            cexpd[IMAG+12*2] = 512.309225281524;
            cexpd[IMAG+13*2] = 512.318281221153;
            cexpd[IMAG+14*2] = 512.325786758292;
            cexpd[IMAG+15*2] = 512.332036369614;
            cexpd[IMAG+16*2] = 512.337249423893;
            cexpd[IMAG+17*2] = 512.341591861241;
            cexpd[IMAG+18*2] = 512.345191550068;
            cexpd[IMAG+19*2] = 512.348149061832;
         }

     double epsilon = 1.0E-12;

//
// Verification test for results.
//
    if(nt<=0)
	{
		//AQUI É VAZIO ASSIM MESMO
    }
	else
	{
		verified = -1;

		for(int it=0;it<nt;it++)
		{
			printf("cksum[REAL + %i] = %.12f cksum[IMAG + %i] = %.12f \n", it*2, cksum[REAL + it*2], it*2, cksum[IMAG + it*2]);

                                       double csumr=(cksum[REAL + it*2]-cexpd[REAL +it*2])/cexpd[REAL +it*2];
			double csumi=(cksum[IMAG + it*2]-cexpd[IMAG +it*2])/cexpd[IMAG +it*2];

			cout << ">>>>>> TESTE " << it << endl;

			//TESTE
			//printf("cksum[REAL + it*2] = %.12f cksum[IMAG + it*2] = %.12f \n", cksum[REAL + it*2], cksum[IMAG + it*2]);
			//cout << "cksum[REAL + it*2] = " << cksum[REAL + it*2] << " cksum[IMAG + it*2] = " << cksum[IMAG + it*2] << endl;
			//printf("cexpd[REAL + it*2] = %.12f cexpd[IMAG + it*2] = %.12f \n", cexpd[REAL + it*2], cexpd[IMAG + it*2]);
			//cout << "cexpd[REAL + it*2] = " << cexpd[REAL + it*2] << " cexpd[IMAG + it*2] = " << cexpd[IMAG + it*2] << endl;

			//TESTE
			//cout << "csumr " << csumr << " csumi " << csumi << " epsilon " <<  epsilon  << endl;

			if ( abs(csumr) <= epsilon || abs(csumi) <= epsilon )
			{
				if(verified==-1)
				{
					verified = 1;
				}
			}

			else
			{
				verified = 0;
			}
		}
    }

	BMResults * results = new BMResults();

	results->PrintVerificationStatus(execClass, verified, BMName);

	return verified;
  }

double FT::GetMFLOPS(std::chrono::duration<double> total_time,int nx,int ny,int nz)
{
	double mflops = 0.0;

	int ntotal = nx*ny*nz;

	double tt = total_time.count();

	if( tt > 0 )
	{
		mflops = 14.8157+7.19641*log(ntotal)
    	      +(5.23518+7.21113*log(ntotal))*niter_default;

		mflops *= ntotal/(tt*1000000.0);
	}

	return mflops;
}

void FT::FftXYZ(int sign, double * x, double * exp1, double * exp2, double * exp3,int n1,int n2,int n3)
{
    int i=0, j=0, k, log;
    int isize3=2,jsize3,ksize3;

	jsize3=isize3*(n1+1);

	ksize3=jsize3*n2;

	int isize1=2,jsize1=2*(n2+1);

    if(timeron)
		timer->Start(3);

    log = Ilog2( n2 );

    if(timeron)
		timer->Start(7);

    for(k=0;k<n3;k++)
		Swarztrauber(sign,log,n1,n2,x,k*ksize3,n1,exp2,scr);

    if(timeron)
		timer->Stop(7);

    log = Ilog2( n1 );

    if(timeron)
		timer->Start(8);

    for(k=0;k<n3;k++)
	{
      for(j=0;j<n2;j++)
	  {
		for(i=0; i<n1;i++)
		{
		  plane[REAL+j*isize1+i*jsize1] = x[REAL+i*isize3+j*jsize3+k*ksize3];
		  plane[IMAG+j*isize1+i*jsize1] = x[IMAG+i*isize3+j*jsize3+k*ksize3];
		}
      }

	  Swarztrauber(sign,log,n2,n1,plane,0,n2,exp1,scr);

      for(j=0;j<n2;j++)
	  {
		for(i=0;i<n1;i++)
		{
			x[REAL+i*isize3+j*jsize3+k*ksize3]=plane[REAL+j*isize1+i*jsize1];
			x[IMAG+i*isize3+j*jsize3+k*ksize3]=plane[IMAG+j*isize1+i*jsize1];
		}
      }
    }

    if(timeron)
		timer->Stop(8);

    log = Ilog2(n3);

    if(timeron)
		timer->Start(9);

    jsize1=2*(n1+1);

    for(k=0;k<n2;k++)
    {
        for(i=0; i<n3;i++)
        {
            for(j=0;j<n1;j++)
            {
            	plane[REAL+j*isize1+i*jsize1] = x[REAL+j*isize3+k*jsize3+i*ksize3];
            	plane[IMAG+j*isize1+i*jsize1] = x[IMAG+j*isize3+k*jsize3+i*ksize3];
            }
        }

      Swarztrauber(sign,log,n1,n3,plane,0,n1,exp3,scr);

        for(i=0; i<n3;i++)
        {
            for(j=0;j<n1;j++)
            {
                    x[REAL+j*isize3+k*jsize3+i*ksize3]=plane[REAL+j*isize1+i*jsize1];
                    x[IMAG+j*isize3+k*jsize3+i*ksize3]=plane[IMAG+j*isize1+i*jsize1];
            }
      }
    }

    if(timeron)
		timer->Stop(9);
    if(timeron)
		timer->Stop(3);
}

void FT::CalculateChecksum(double * csum, int csmffst, int iterN,double * u,int d1, int d2, int d3)
{
    int i, ii, ji, ki;

    int isize3=2,
    	jsize3=isize3*(d1+1),
        ksize3=jsize3*d2;

	csum[REAL+csmffst] = 0.0;
    csum[IMAG+csmffst] = 0.0;

    double csumr = 0.0, csumi = 0.0;

    for(i=1;i<=1024;i++)
	{
      ii = (1*i)%d3;
      ji = (3*i)%d1;
      ki = (5*i)%d2;
      csumr+=u[REAL+ji*isize3+ki*jsize3+ii*ksize3];
      csumi+=u[IMAG+ji*isize3+ki*jsize3+ii*ksize3];

	  //printf("csumr = %.16f\n", csumr);
	  //printf("csumr = %.16f\n", csumi);
    }

    csum[REAL+csmffst] = csumr/(d1*d2*d3);
    csum[IMAG+csmffst] = csumi/(d1*d2*d3);
}

void FT::setupThreads()
{
    master = this;

    if(numThreads>nz)
            numThreads=nz;
    if(numThreads>nx)
            numThreads=nx;

    int * interval1 = new int[numThreads];
    int * interval2 = new int[numThreads];

    SetInterval(numThreads, nz, interval1);
    SetInterval(numThreads, nx, interval2);

    int ** partition1 = (int**)malloc(sizeof(int)*numThreads);
    int ** partition2 = (int**)malloc(sizeof(int)*numThreads);

    for(int i = 0; i < numThreads; i++)
    {
        partition1[i] = (int*)malloc(sizeof(int)*2);
        partition2[i] = (int*)malloc(sizeof(int)*2);
    }

    SetPartition(0, interval1, partition1, numThreads);
    SetPartition(0, interval2, partition2, numThreads);

    /*
    doFFT = new FFTThread[num_threads];
    doEvolve = new EvolveThread[num_threads];

    for(int ii=0;ii<num_threads;ii++)
    {
      doFFT[ii] = new FFTThread(ft,partition1[ii][0],partition1[ii][1],
                   partition2[ii][0],partition2[ii][1]);
      doFFT[ii].id=ii;
      doFFT[ii].start();

      doEvolve[ii] = new EvolveThread(ft,partition2[ii][0],partition2[ii][1]);
      doEvolve[ii].id=ii;
      doEvolve[ii].start();
    }
    */
}