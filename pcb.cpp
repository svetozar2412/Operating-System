#include "pcb.h"
#include "SCHEDULE.h"
#include "Queue.h"
#include "timer.h"
#include <iostream.h>

ID PCB::counter=1;
Queue PCB::allPCBs=Queue();
volatile PCB* PCB::running=0;
volatile Idle* PCB::idle=0;
Thread* PCB::main=0;


void PCB::wrapper()
{
	((PCB*)PCB::running)->getMyThread()->run();
#ifndef BCC_BLOCK_IGNORE
    lockInterrupts
#endif
    Node* temp=(((PCB*)PCB::running)->getWaiting())->getHead();
    Node* last=0;

    while(temp)
    {
        temp->pcb->setState(READY);
        Scheduler::put((PCB*)(temp->pcb));
        last=temp;
        temp=temp->next;
        delete last;
        last=0;
    }
    ((PCB*)PCB::running)->setState(FINISHED);//da li je samo ovo dovoljno?
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
    dispatch();
}

PCB::PCB(Thread* myThread,StackSize stackSize,Time time)
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts;
#endif
    this->myThread=myThread;
    this->timeSlice=time;
    this->waiting=new Queue();
    this->state=NEW;
    this->returnFromBlockValue=1;
    this->criticalSectionCounter=0; //DODATO ZBOG ZABRANE PREOTIMANJA
    this->id=counter++;
    unsigned size=((stackSize<=65536)?(stackSize/2):(65536/2));
    this->stack=new unsigned[size];
#ifndef BCC_BLOCK_IGNORE
    stack[size-1]=0x200;
    stack[size - 2] = FP_SEG(&wrapper);
    stack[size - 3] = FP_OFF(&wrapper);
    ss = FP_SEG(stack + size - 12);
    sp = FP_OFF(stack + size - 12);
#endif
    bp = sp;
    allPCBs.addNode(this);
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

PCB::~PCB()
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
    if(this->stack)delete [] this->stack;
    if(this->waiting)delete this->waiting;
    //izbaci PCB iz liste svih PCB-ova
    allPCBs.deleteNode(this);//da li moze samo ovo,jer sta ako je ta nit u Scheduler-u
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

void PCB::initMainThread()
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
	main=new Thread(1024,1);
	main->myPCB->setState(RUNNING);
    PCB::running =(PCB*)(main->myPCB);
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

void PCB::initIdleThread()
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
	idle=new Idle();
#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif

}
