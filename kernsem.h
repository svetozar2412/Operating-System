#ifndef _kernsem_h_
#define _kernsem_h_

#include "semaphor.h"
#include "pcb.h"
#include <iostream.h>

struct SemNode
{
    KernelSem* sem;
    SemNode* next;
    SemNode(KernelSem* s)
    {
        sem=s;
        next=0;
    }
};

struct WaitingPCBNode
{
    PCB* pcb;
    Time remainingTime;
    WaitingPCBNode* next;
    WaitingPCBNode(PCB* p,Time t)
    {
        pcb=p;
        remainingTime=t;
        next=0;
    }
};


class KernelSem
{
private:
    int value;
    WaitingPCBNode* blockedPCBs;
    Semaphore* mySem;
public:
    static volatile SemNode* allSemaphores;
    friend class Timer;
    KernelSem(Semaphore*,int);
    ~KernelSem();
    int wait(Time maxTimeToWait);
    void signal();
    int getVal () const
    {return value;}
};

#endif
