#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.h"
#include "timer.h"
#include "Queue.h"
#include <iostream.h>

Thread::Thread(StackSize ss,Time t)
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
    myPCB=new PCB(this,ss,t);
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
}

Thread::~Thread()
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
    if(myPCB)
    {
    this->waitToComplete();
    delete myPCB;
    myPCB=0;
    }
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
}

void Thread::start()
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
    if(this->myPCB->getState()==NEW)
    {
    this->myPCB->setState(READY);
    Scheduler::put((PCB*)this->myPCB);
    }
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
}

void Thread::waitToComplete()
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
	if(myPCB->getState()==NEW || myPCB->getState()==RUNNING ||
	 myPCB->getState()==FINISHED || myPCB==0/*|| this->myPCB==PCB::main->myPCB || this->myPCB==PCB::idle->myPCB*/) //treba li main da bude ovde? //mislim da ne treba,jer niko ne moze da pozove to za main
	 {
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
		return;
	 }

    this->myPCB->getWaiting()->addNode((PCB*)(PCB::running));//dodaj PCB u listu cekanja
    ((PCB*)(PCB::running))->setState(BLOCKED);
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
    dispatch();
}

void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
    Timer::switchContextOnDemand=1;
    Timer::timer();
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
}

ID Thread::getId()
{
    return this->myPCB->getID();
}

ID Thread::getRunningId()
{
    return ((PCB*)(PCB::running))->getID();
}

Thread * Thread::getThreadById(ID id)
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
    Node* temp=PCB::allPCBs.getHead();
    while(temp)
    {
        if(temp->pcb->getID()==id)break;
        temp=temp->next;
    }
    if(temp)
    {
#ifndef BCC_BLOCK_IGNORE
    	unlockInterrupts
#endif
    	return temp->pcb->getMyThread();
	}
	else
	{
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif
		return 0;
	}
    
}

