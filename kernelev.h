#ifndef _kernelev_h_
#define _kernelev_h_

#include "Event.h"
class PCB;
class IVTEntry;

class KernelEv {
public:
~KernelEv();
KernelEv(Event *, IVTNo);
void wait();
void signal();

private:
Event* myEvent;
IVTEntry* myIVTEntry;
int value;
PCB* creator;
PCB* blockedThread;
};
#endif
