#ifndef _PCB_H_
#define _PCB_H_

#include "idle.h"

class Thread;
class Queue;
enum StateOfThread {NEW,READY,RUNNING,BLOCKED,FINISHED};

class PCB
{
    private:
    static ID counter;
    ID id;
    unsigned *stack;
    unsigned ss;
    unsigned sp;
    unsigned bp;
    Time timeSlice;
    StateOfThread state;
    Queue* waiting;
    Thread* myThread;

    friend class Timer;
    friend class Idle;

    public:
    int returnFromBlockValue;
    unsigned criticalSectionCounter;
    static void initMainThread();
    static void initIdleThread();
    static void wrapper();
    PCB(Thread *mt,StackSize stackSize,Time time);
    ~PCB();
    void setState(StateOfThread s) {this->state=s;}
    StateOfThread getState() {return this->state;}
    ID getID() {return this->id;}
    Queue* getWaiting(){return waiting;}
    Thread* getMyThread(){return myThread;}
    static Queue allPCBs;
    static volatile PCB* running;
    static volatile Idle* idle;
    static Thread* main;
};

#endif
