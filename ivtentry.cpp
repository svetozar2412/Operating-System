#include "ivtentry.h"
#include "Event.h"
#include "kernelev.h"
#include <dos.h>
#include "timer.h"

IVTEntry* IVTEntry::allEntries[256]={0};//mozda ne moze ovde da se inicijalizuje

IVTEntry::IVTEntry(IVTNo numEntry, pInterrupt newR)
{
#ifndef BCC_BLOCK_IGNORE
		lockInterrupts
#endif
#ifndef BCC_BLOCK_IGNORE
    this->oldRoutine = getvect(numEntry);
#endif
    this->entry=numEntry;
#ifndef BCC_BLOCK_IGNORE
    setvect(numEntry, newR);
#endif
    IVTEntry::allEntries[numEntry]=this;
    this->myKEv=0;
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

IVTEntry::~IVTEntry()
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
#ifndef BCC_BLOCK_IGNORE
    setvect(this->entry,this->oldRoutine);
#endif
    IVTEntry::allEntries[this->entry]=0;
#ifndef BCC_BLOCK_IGNORE
    unlockInterrupts
#endif
}

void IVTEntry::signalE()
{
	this->myKEv->signal(); //OVO PRIJAVLJUJE GRESKU?
}
