#include "thread.h"
#include "pcb.h"
#include "macros.h"

void dispatch() {
	LOCK
	PCB::dispatchImpl();
	UNLOCK
}

void Thread::waitToComplete() {
	LOCK
	myPCB->waitToComplete();
	UNLOCK
}

void Thread::start() {
	LOCK
	myPCB->start();
	UNLOCK
}

Thread* Thread::getThreadById(ID id) {
	Thread* t;
	LOCK
	t = PCB::getThreadById(id);
	UNLOCK
	return t;
}

ID Thread::getId() {
	ID id;
	LOCK
	id = myPCB->getId();
	UNLOCK
	return id;
}

ID Thread::getRunningId() {
	ID id;
	LOCK
	id = PCB::running->getId();
	UNLOCK
	return id;
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	LOCK
	myPCB = new PCB(stackSize, timeSlice, this);
	UNLOCK
}

Thread::~Thread() {
	waitToComplete();
	LOCK
	delete myPCB;
	UNLOCK
}
