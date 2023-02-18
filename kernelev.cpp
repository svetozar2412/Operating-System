#include "kernelev.h"
#include "SCHEDULE.h"
#include "pcb.h"
#include "ivtentry.h"
#include "iostream.h"
#include "timer.h"

KernelEv::KernelEv(Event* e,IVTNo num)
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
    this->myEvent=e;
    this->myIVTEntry=IVTEntry::allEntries[num];
    this->myIVTEntry->myKEv=this;
    this->creator=(PCB*)(PCB::running);
    this->blockedThread=0;
    this->value=1;
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

KernelEv::~KernelEv()
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
    this->myIVTEntry->myKEv=0;
    this->myIVTEntry=0;
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

void KernelEv::wait()
{

#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
		if(((PCB*)(PCB::running))==this->creator){
			if(--this->value<0){
			((PCB*)(PCB::running))->setState(BLOCKED);
			this->blockedThread=(PCB*)(PCB::running);
			dispatch();
			}
		}
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif

}

void KernelEv::signal()
{

#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
		if(this->value++<0){
			this->creator->setState(READY);
			Scheduler::put((PCB*)(this->creator));
			this->blockedThread=0;
		}
		else
		{
			this->value=1;
		}
#ifndef BCC_BLOCK_IGNORE
		unlockInterrupts
#endif

}
