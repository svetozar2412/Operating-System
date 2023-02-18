#ifndef _timer_h_
#define _timer_h_

#include <dos.h>

extern void tick();
class Idle;
extern volatile unsigned intCounter;

#define lockInterrupts asm {pushf;cli;}
#define unlockInterrupts asm popf

/*
#define lockInterrupts asm {\
pushf;\
cli;\
push ax;\
mov ax,intCounter;\
inc ax;\
mov intCounter,ax;\
pop ax;\
popf;\
}
*/

/*
#define unlockInterrupts asm {\
pushf;\
cli;\
push ax;\
mov ax,intCounter;\
dec ax;\
mov intCounter,ax;\
pop ax;\
popf;}
*/

typedef void interrupt (*Interrupt)(...);
class Timer
{
public:
static volatile int switchContextOnDemand;
static Interrupt oldRoutine;
static void interrupt timer(...);
static void inicTimer();
static void inicSystem();
static void restoreTimer();
static void restoreSystem();
static void lock(),unlock();
static void semaphorePass();
};
#endif
