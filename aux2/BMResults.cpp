/*
 * BMResults.cpp
 *
 *  Created on: 31 de mar de 2016
 *      Author: guilherme
 */

#include "BMResults.h"

BMResults::BMResults()
{
	tmSent = 0.0;
	tmReceived = 0.0;
	RecArrSize = 0;
}

BMResults::BMResults(string bname,
				  	 char   CLASS,
					 int    bn1,
					 int    bn2,
					 int    bn3,
					 int    bniter,
					 std::chrono::duration<double> btime,
					 double bmops,
					 string boptype,
					 int    passed_verification,
					 bool bserial,
					 int num_threads,
					 int bid)
{
	tmSent = 0.0;
	tmReceived = 0.0;
	RecArrSize = 0;

	pid=bid;
	name=bname;
	clss=CLASS;
	n1=bn1;
	n2=bn2;
	n3=bn3;
	niter=bniter;
	time=btime;
	mops=bmops;
	optype=boptype;
	verified=passed_verification;
	serial=bserial;
	numthreads=num_threads;
}

BMResults::~BMResults()
{
	// TODO Auto-generated destructor stub
}

void BMResults::PrintVerificationStatus(char clss, int verified, string BMName)
{	
	if (clss == 'U' || verified == -1)
    {
      verified = -1;

      cout << " Problem size unknown" << endl;

      cout << BMName << "." << clss << ": Verification Not Performed" << endl;
    }
    else if (verified==1)
      cout << BMName << "." << clss << ": Verification Successful" << endl;
    else
      cout << BMName << "." << clss << ": Verification Failed" << endl;
}

void BMResults::Print()
{
    //DecimalFormat fmt = new DecimalFormat("0.000");
    cout << "***** NAS Parallel Benchmarks  C++ version (NPB3_0_CPP) " << name << " *****" << endl;

    cout << "* Class             = " << clss << endl;

    if( n2 == 0 && n3 == 0 )
    	cout << "* Size              = " << n1 << endl;
	else
		cout << "* Size              = " << n1 << " X " << n2 << " X " << n3 << endl;

    cout << "* Iterations        = " << niter << endl;
    cout << "* Time in seconds   = " << /*fmt.format(*/time.count()/*)*/ << endl;
    cout << "* ACCTime           = " << /*fmt.format(*/acctime/*)*/ << endl;
    cout << "* Mops total        = " << /*fmt.format(*/mops/*)*/ << endl;

    cout << "* Operation type    = " << optype << endl;

    if(verified==1)
    	cout << "* Verification      = Successful" << endl;
    else
    {
    	if(verified==0)
    		cout << "* Verification      = Failed" << endl;
    	else
    		cout << "* Verification      = Not Performed" << endl;
    }

    if(!serial)
    {
    	cout << "* Threads requested = " << numthreads << endl;
    	cout << "* " << endl;
	}

    cout << "* Please send all errors/feedbacks to:" << endl;
    cout << "* NPB Working Team" << endl;
    cout << "* npb@nas.nasa.gov" << endl;
    cout <<"*****************************************************************" << endl;

    /*
     *
     * ESSA PARTE AQUI EM BAIXO AINDA NÃO SEI PARA QUE QUE SERVE.
     * DESCOBRIREI COM O TEMPO (EU ESPERO)
     *
     */
    /*
    if(out!=null){
      try{
 	outline="***** NAS Parallel Benchmarks Java version (NPB3_0_JAV) "
	              +name+" Report *****";
        out.write(outline,0,outline.length());
        out.newLine();
        outline="Class           = "+String.valueOf(clss);
        out.write(outline,0,outline.length());
        out.newLine();
        if( n2 == 0 && n3 == 0 ){
          outline="Size            = "+String.valueOf(n1);
        }else{
          outline="Size            = "+String.valueOf(n1)+" X "+
	                               String.valueOf(n2)+" X "+
				       String.valueOf(n3);
        }
        out.write(outline,0,outline.length());
        out.newLine();
        outline="Iterations      = "+String.valueOf(niter);
        out.write(outline,0,outline.length());
        out.newLine();
        outline="Time in seconds = "+String.valueOf(fmt.format(time));
        out.write(outline,0,outline.length());
        out.newLine();
        outline="ACCTime         = "+String.valueOf(fmt.format(acctime));
        out.write(outline,0,outline.length());
        out.newLine();
        outline="Mops total      = "+String.valueOf(fmt.format(mops));
        out.write(outline,0,outline.length());
        out.newLine();
        outline="Operation type  = "+String.valueOf(optype);
        out.write(outline,0,outline.length());
        out.newLine();
        if(verified==1)      outline="Verification    = Successful";
        else if(verified==0) outline="Verification Failed";
        else                 outline="Verification Not Performed";
        out.write(outline,0,outline.length());
        out.newLine();

    	outline="\n Please send all errors/feedbacks to:";
        out.write(outline,0,outline.length());
        out.newLine();
        outline=" NPB Working Team";
        out.write(outline,0,outline.length());
        out.newLine();
        outline=" npb@nas.nasa.gov\n";
        out.write(outline,0,outline.length());
        out.newLine();
	out.flush();
      }catch(Exception e){
        System.err.println("Res.print: write file: "+e.toString());
      }
    }  */
}
