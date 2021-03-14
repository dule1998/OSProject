#include "kernsem.h"
#include "pcb.h"
#include "sleeping.h"
#include "state.h"
#include "schedule.h"
#include "pcbqueue.h"


SleepingList* KernelSem::sleeping = 0;

KernelSem::KernelSem(int init) {
	blocked = new PCBQueue();
	if(init < 0)
		init = 0;
	v = init;
}

KernelSem::~KernelSem() {
	if (v < 0)
		signal(-v);
}

int KernelSem::signal(int n) {
	if (n < 0)
		return n;

	int numAwaken = 0;
	int startingN = n;
	if (n == 0)
		n++;


	for (int i = 0; i < n; i++) {
		if (v >= 0) {
			v += n-i;
			break;
		}
		v++;
		PCB* pcb = blocked->takeHead();
		if (pcb) {
			numAwaken++;
			Scheduler::put(pcb);
			pcb->state.setStateReady();
			sleeping->remove(pcb);
			pcb->ksem = 0;
			pcb->semReturn = 1;
		}
	}

	if (startingN > 0)
		return numAwaken;
	else
		return 0;
}

int KernelSem::wait(Time maxTimeToWait) {
	v--;
	if (v < 0) {
		PCB::running->state.setStateBlocked();
		PCB::running->ksem = this;
		if (maxTimeToWait > 0)
			sleeping->add(PCB::running, maxTimeToWait);
		blocked->add(PCB::running);
		dispatch();
	}
	else
		PCB::running->semReturn = 1;

	return PCB::running->semReturn;
}

void KernelSem::remove(PCB* pcb) {
	v++;
	blocked->remove(pcb);
}

int KernelSem::getVal() {
	return v;
}



