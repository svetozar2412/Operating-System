#ifndef _QUEUE_H_
#define _QUEUE_H_

//#include "pcb.h"

class PCB;

struct Node
{
    PCB* pcb;
    Node* next;
    Node(PCB* data)
    {
        pcb=data;
        next=0;
    }
};

class Queue
{
private:
    Node* first;
public:
    Queue();
    ~Queue();
    PCB* getNode();
    void addNode(PCB* pcb);
    int getSize();
    void deleteNode(PCB*);
    Node* getHead();
};

#endif
