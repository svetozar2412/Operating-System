#include "Queue.h"

int Queue::getSize()
{
    Node* temp=first;
    int cnt=0;
    while(temp)
    {
        cnt++;
        temp=temp->next;
    }
    return cnt;
}

Queue::Queue()
{
    first=0;
}

Queue::~Queue()
{
    Node* temp=first;
    while(temp)
    {
        Node* last=temp;
        temp=temp->next;
        delete last;
    }
    first=0;
}

PCB* Queue::getNode()
{
    if(!first)
    {
        return 0;
    }
    else
    {
        Node* node=first;
        PCB* temp=first->pcb;
        first=first->next;
        delete node;
        return temp;
    }
    
}

void Queue::addNode(PCB* pcb)
{
    if(!first)
    {
        first=new Node(pcb);
    }
    else
    {
        Node* temp=first;
        Node* last=0;
        while(temp && temp->pcb!=pcb)//Do poslednjeg elementa ili dok ne naidjemo na isti PCB
        {                            //ako vec postoji,ne dodajemo ga
            last=temp;
            temp=temp->next;
        }
        if(temp)return; //ako vec postoji,ne dodavati
        else
        {
            Node* node=new Node(pcb);
            last->next=node;
            node->next=temp;
        }
    }
}

void Queue::deleteNode(PCB* pcb)
{
    Node* temp=first;
    Node* last=0;
    if(!first) return;
    else if(first->pcb==pcb)
    {
        first=first->next;
        delete temp;
    }
    else
    {
        while(temp)
        {
        if(temp->pcb==pcb)
        {
            last->next=temp->next;
            delete temp;
        }
        last=temp;
        temp=temp->next;
        }
    }
}

Node* Queue::getHead()
{
    return first;
}
