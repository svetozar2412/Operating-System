#include "kernsem.h"
#include "SCHEDULE.h"
#include <iostream.h>
#include "timer.h"

volatile SemNode* KernelSem::allSemaphores = 0;

KernelSem::KernelSem(Semaphore* sem, int init) {

#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
	this->mySem = sem;
	this->value = init;
	this->blockedPCBs = 0;
	if (allSemaphores == 0) {
		allSemaphores = new SemNode(this);
	} else {
		SemNode* temp = (SemNode*) (KernelSem::allSemaphores);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = new SemNode(this);
	}
#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif

}

KernelSem::~KernelSem() { //ispravljene greske

#ifndef BCC_BLOCK_IGNORE
lockInterrupts
#endif
	WaitingPCBNode* tempX = blockedPCBs;
	WaitingPCBNode* tempY = 0;
	while (tempX) {
		tempY = tempX;
		tempX = tempX->next;
		tempY->pcb->setState(READY);
		Scheduler::put(tempY->pcb);
		delete tempY;
	}
	blockedPCBs = 0;

	SemNode* temp = (SemNode*) (KernelSem::allSemaphores);
	SemNode* prev=0;
	if (temp->next == 0) { //ostao je jedini semafor
		delete temp;
		KernelSem::allSemaphores = 0;
	} else {
		while (temp->sem != this) //temp->next u nekom trenutku bude =0,pa se pristupa 0->sem
		{
			prev=temp;
			temp = temp->next;
		}

		//SemNode* del = temp;
		if(temp==allSemaphores) allSemaphores=allSemaphores->next;
		SemNode* x = temp->next;
		if(prev))prev->next = x;
		delete temp;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif
}

int KernelSem::wait(Time t) {

#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
	if (--(this->value) < 0) {
		if (!blockedPCBs) {
			this->blockedPCBs = new WaitingPCBNode((PCB*) (PCB::running), t);
		} else {
			WaitingPCBNode* temp = blockedPCBs;
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = new WaitingPCBNode((PCB*) (PCB::running), t);
		}
		((PCB*) (PCB::running))->setState(BLOCKED);

		dispatch();
		//OVDE IZLAZIM AKO SAM SE BLOKIRAO!!!!
		 int x = PCB::running->returnFromBlockValue;
		 PCB::running->returnFromBlockValue = 0;
#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif

		 return x;
	}
#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif
		return 0;
}

void KernelSem::signal()
{
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif
		if ((this->value)++ < 0) {
			WaitingPCBNode* temp = blockedPCBs;
			blockedPCBs = blockedPCBs->next;
			((PCB*) (temp->pcb))->setState(READY);
			PCB *x = temp->pcb;
			Scheduler::put((PCB*) x);
			x->returnFromBlockValue = 1;
			delete temp;
		}
#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif
}
