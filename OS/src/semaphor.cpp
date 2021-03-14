#include "semaphor.h"
#include "macros.h"
#include "pcb.h"
#include "kernsem.h"

Semaphore::Semaphore(int init) {
	LOCK
	myImpl = new KernelSem(init);
	UNLOCK
}

Semaphore::~Semaphore() {
	LOCK
	delete myImpl;
	UNLOCK
}

int Semaphore::wait(Time maxTimeToWait) {
	int v;
	LOCK
	v = myImpl->wait(maxTimeToWait);
	UNLOCK
	return v;
}

int Semaphore::signal(int n) {
	int v;
	LOCK
	v = myImpl->signal(n);
	UNLOCK
	return v;
}

int Semaphore::val() const {
	int v;
	LOCK
	v = myImpl->getVal();
	UNLOCK
	return v;
}



