#ifndef _ivtentry_h_
#define _ivtentry_h_

typedef void interrupt (*pInterrupt)(...);

#include "Event.h"

class KernelEv;
class IVTEntry
{
    private:
    unsigned char entry;
    KernelEv* myKEv;
    friend KernelEv;
    static IVTEntry* allEntries[256];//mozda ne moze ovde da se inicijalizuje
    
    public:
    IVTEntry(IVTNo i,pInterrupt p);
    ~IVTEntry();
    void signalE();
    pInterrupt oldRoutine;
};

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
newEntry##numEntry.signalE();\
if (callOld == 1)\
newEntry##numEntry.oldRoutine();\
}

#endif
