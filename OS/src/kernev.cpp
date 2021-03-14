#include "kernev.h"
#include "state.h"
#include "schedule.h"
#include "ivtentry.h"
#include "pcb.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	this->ivtNo = ivtNo;
	v = 0;
	myPCB = PCB::running;
	ivtEntry = IVTEntry::ivtEntries[ivtNo];
	if (ivtEntry)
		if (!ivtEntry->event)
			ivtEntry->event = this;


}

KernelEv::~KernelEv() {
	if(v < 0)
		signal();

	if (ivtEntry)
		if (this == ivtEntry->event)
			ivtEntry->event = 0;

}

void KernelEv::wait() {
	if (PCB::running != myPCB)
		return;

	v--;
	if(v == -1) {
		PCB::running->state.setStateBlocked();
		dispatch();
	}
}

void KernelEv::signal() {
	v++;
	if(v == 0) {
		myPCB->state.setStateReady();
		Scheduler::put(myPCB);
	} else
		v = 1;
}


