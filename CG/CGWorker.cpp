#include "CGWorker.h"

CGWorker::CGWorker()
{

}

CGWorker::CGWorker(CG * cg,int st,int end)
{
    Init((CGBase *)cg);
    start1=st;
    end1=end;
    done = true;
    //setDaemon(true);
    //setPriority(Thread.MAX_PRIORITY);
    master=cg;
  }

bool CGWorker::IsDone()
{
    return done;
}

int CGWorker::GetTaskOrder()
{
    return taskOrder;
}

double CGWorker::GetAlpha()
{
    return alpha;
}

double CGWorker::GetBeta()
{
    return beta;
}

int CGWorker::GetId()
{
    return id;
}

void CGWorker::SetDone(bool d)
{
    done = d;
}

void CGWorker::SetTaskOrder(int o)
{
    taskOrder = o;
}

void CGWorker::SetAlpha(double a)
{
    alpha = a;
}

void CGWorker::SetBeta(double b)
{
    beta = b;
}

void CGWorker::SetId(int i)
{
    id = i;
}

void CGWorker::Init(CGBase * cg)
{
    //initialize shared data
    dmaster = cg->GetDmaster();
    rhomaster = cg->GetRhomaster();
    rnormmaster = cg->GetRnormmaster();
    colidx = cg->GetColidx();
    rowstr = cg->GetRowstr();
    a = cg->GetA();
    p = cg->GetP();
    q = cg->GetQ();
    r = cg->GetR();
    x = cg->GetX();
    z = cg->GetZ();
}

void CGWorker::join()
{
    workingThread->join();
}

void CGWorker::start()
{
    workingThread = new thread(&CGWorker::run, this);
}

void CGWorker::run()
{
    //cout << "Thread " << id << " running." << endl;

    int state=0;

    for(;;)
    {
        while(done){}

        switch(taskOrder)
        {
            case 0:
                  Step0();
                  break;
            case 1:
                  Step1();
                  break;
            case 2:
                  Step2();
                  break;
            case 3:
                  Step3();
                  break;
            case 4:
                  EndWork();
                  break;
        }

        done=true;
    }
}

void CGWorker::Step0()
{
for(int j=start1;j<=end1;j++){
      double sum = 0.0;
      for(int k=rowstr[j];k<rowstr[j+1];k++){
    sum = sum + a[k]*p[colidx[k]];
      }
      q[j] = sum;
    }
    double sum = 0.0;
    for(int j=start1;j<=end1;j++) sum += p[j]*q[j];
    dmaster[id]=sum;
}

void CGWorker::Step1()
{
for(int j=start1;j<=end1;j++){
      z[j] = z[j] + alpha*p[j];
      r[j] = r[j] - alpha*q[j];
    }
//---------------------------------------------------------------------
//  rho = r.r
//  Now, obtain the norm of r: First, sum squares of r elements locally...
//---------------------------------------------------------------------
    double rho = 0.0;
    for(int j=start1;j<=end1;j++) rho += r[j]*r[j];
    rhomaster[id]=rho;
}

void CGWorker::Step2()
{
for(int j=start1;j<=end1;j++) p[j]=r[j]+beta*p[j];
}

void CGWorker::Step3()
{
double rho = 0.0;
    for(int j=start1;j<=end1;j++){
      q[j] = 0.0;
      z[j] = 0.0;
      r[j] = x[j];
      p[j] = x[j];
      rho += x[j]*x[j];
    }
    rhomaster[id]=rho;
}

void CGWorker::EndWork()
{

//---------------------------------------------------------------------
//  Compute residual norm explicitly:  ||r|| = ||x - A.z||
//  First, form A.z
//  The partition submatrix-vector multiply
//---------------------------------------------------------------------
    for(int j=start1;j<=end1;j++){
      double sum = 0.0;
      for(int k=rowstr[j];k<=rowstr[j+1]-1;k++){
    sum += a[k]*z[colidx[k]];
      }
      r[j] = sum;
    }
//---------------------------------------------------------------------
//  At this point, r contains A.z
//---------------------------------------------------------------------
    double sum = 0.0;
    for(int j=start1;j<=end1;j++) sum+=(x[j]-r[j])*(x[j]-r[j]);
    rnormmaster[id]=sum;
}