#include "kernsem.h"
#include "timer.h"

Semaphore::Semaphore(int init)
{
		myImpl=new KernelSem(this,init);
}

Semaphore::~Semaphore()
{
    delete myImpl;//???
}

int Semaphore::wait(Time time)
{
    return myImpl->wait(time);
}

void Semaphore::signal()
{
    myImpl->signal();
}

int Semaphore::val() const
{
    return this->myImpl->getVal();
}
