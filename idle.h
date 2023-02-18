#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

class Idle: public Thread {
	friend class Timer;
public:
Idle();
virtual void run();//mora li virtual?
void start();
PCB* getPCB();
};
#endif
