#include <iostream.h>
#include <dos.h>
#include "timer.h"
#include "kernsem.h"
#include "pcb.h"
#include "SCHEDULE.h"

#include <assert.h>

volatile int Timer::switchContextOnDemand = 0;
Interrupt Timer::oldRoutine = 0;
volatile unsigned intCounter = 0;
volatile unsigned tsp, tss, tbp, tax;

void Timer::lock() {

}

void Timer::unlock() {
}

void Timer::inicTimer() {
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif

#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(8);
	setvect(8, timer);
#endif

#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif
}

void Timer::restoreTimer() {
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
	setvect(8, oldRoutine);
	unlockInterrupts
#endif
}

void Timer::semaphorePass() {
	SemNode* temp = (SemNode*) (KernelSem::allSemaphores);
	WaitingPCBNode* tempX = 0;
	WaitingPCBNode* tempY = 0;

	while (temp) {
		tempX = temp->sem->blockedPCBs;
		tempY = 0;
		while (tempX) {
			if (tempX->remainingTime == 1) {
				tempX->pcb->returnFromBlockValue = 2;
				if (tempX == temp->sem->blockedPCBs) {
					tempY = tempX;
					(temp->sem)->blockedPCBs = tempX =
							((temp->sem)->blockedPCBs)->next;
					tempY->pcb->state = READY;
					Scheduler::put(tempY->pcb);
					delete tempY;
					tempY = 0;
				} else {
					/*tempY = tempX;
					 tempX = tempX->next;
					 tempY->pcb->state = READY;
					 Scheduler::put(tempY->pcb);
					 delete tempY;*/
					WaitingPCBNode* tempMiddle = tempX;
					tempX = tempX->next;
					tempMiddle->pcb->state = READY;
					Scheduler::put(tempMiddle->pcb);
					delete tempMiddle;
					tempY->next = tempX;
				}
				(temp->sem->value)++;
			} else if (tempX->remainingTime > 1) {
				tempX->remainingTime--;
				tempY = tempX;
				tempX = tempX->next;
			} else {
				//ako je remainingTime=0,samo ce se preskociti PCB
				tempY = tempX;
				tempX = tempX->next;
			}
		}
		temp = temp->next;
	}
}

void interrupt Timer::timer(...) {

	PCB::running->criticalSectionCounter = intCounter; //ZGOG ZABRANE PREUZIMANJA

	static int cnt = PCB::running->timeSlice;
	if (!switchContextOnDemand && intCounter == 0) {
		Timer::semaphorePass();
	}

	if (!switchContextOnDemand) {
		/*
#ifndef BCC_BLOCK_IGNORE
		cout << "[NOT DISPATCH]\n";
		asm cli
#endif
*/
		Timer::oldRoutine();
		tick();
		if (PCB::running->timeSlice != 0 && cnt >= 0) {
			cnt -= 1;
		}
	}

	if ((cnt == 0 && PCB::running->timeSlice != 0 && intCounter == 0)
			|| switchContextOnDemand) {

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif

		/*
#ifndef BCC_BLOCK_IGNORE
		cout << "[PROMENA KONTEKSTA]"<<intCounter<<endl;
		asm cli
#endif
*/

		(PCB::running)->sp = tsp;
		(PCB::running)->ss = tss;
		(PCB::running)->bp = tbp;

		if (((PCB*) (PCB::running))->state != FINISHED
				&& ((PCB*) (PCB::running))->state != BLOCKED
				&& ((PCB*) (PCB::running) != (PCB::idle)->myPCB)) {
			((PCB*) (PCB::running))->state = READY;
			Scheduler::put((PCB*) (PCB::running));
		}
		(PCB::running) = Scheduler::get();
		if ((PCB::running) == 0) {
			(PCB::running) = ((PCB::idle)->myPCB);

		} else {
			((PCB*) (PCB::running))->state = RUNNING;
		}

		intCounter = PCB::running->criticalSectionCounter; //ZGOG ZABRANE PREUZIMANJA
		cnt = (PCB::running)->timeSlice;
		tsp = (PCB::running)->sp;
		tss = (PCB::running)->ss;
		tbp = (PCB::running)->bp;

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp , tbp
		}
#endif

	}

	switchContextOnDemand = 0;
}

void Timer::inicSystem() {
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif

	PCB::initMainThread();
	PCB::initIdleThread();
	Timer::inicTimer();

#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif
}

void Timer::restoreSystem() {
#ifndef BCC_BLOCK_IGNORE
	lockInterrupts
#endif

	Timer::restoreTimer();
	delete PCB::main;
	delete PCB::idle;

#ifndef BCC_BLOCK_IGNORE
	unlockInterrupts
#endif
}

