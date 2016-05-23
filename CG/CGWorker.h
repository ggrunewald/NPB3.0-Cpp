#ifndef CGWORKER_H
#define CGWORKER_H

#include <thread>

#include "CGBase.h"

class CGWorker : public CGBase
{
private:
    bool done;
    int id;
    int taskOrder;
    double alpha, beta;
    int start1, end1;

    thread * workingThread;

public:
    CGWorker();
    CGWorker(CG * cg,int st,int end);

    bool IsDone();
    int GetTaskOrder();
    double GetAlpha();
    double GetBeta();
    int GetId();

    void SetDone(bool d);
    void SetTaskOrder(int o);
    void SetAlpha(double a);
    void SetBeta(double b);
    void SetId(int i);

    void Init(CGBase * cg);

    void start();
    void run();
    void join();

    void Step0();
    void Step1();
    void Step2();
    void Step3();
    void EndWork();
};

#endif