#include "idle.h"
#include "pcb.h"
#include "thread.h"
#include "timer.h"
#include <iostream.h>

Idle::Idle():Thread(2048, 1){
}

void Idle::run()
{
	long x=0;
	x++;
	while (1)
	{
		x++;
		//cout<<"IDLE"<<intCounter<<endl;
	}
}

void Idle::start()
{

}

PCB* Idle::getPCB()
{
	return this->myPCB;
}
