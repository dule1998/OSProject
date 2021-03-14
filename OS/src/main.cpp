#include "sleeping.h"
#include "pcb.h"
#include "pcbqueue.h"
#include "macros.h"
#include "state.h"
#include "kernsem.h"
#include "idle.h"

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	PCB::allThreads = new PCBQueue();
	KernelSem::sleeping = new SleepingList();
	PCB::mainThread = new Thread(0);
	PCB::running = PCB::mainThread->myPCB;
	PCB::running->state.setStateReady();

	PCB::idlePCB = 0; // bio je postavljen na ovaj PCB gore u konstruktoru PCB
	PCB::idle = new Idle();
	PCB::idle->start();

	LOCK
	PCB::oldTimerIntr = GETVECT(8);
	UNLOCK
	LOCK
	SETVECT(8, &PCB::timerIntr);
	UNLOCK

	int result = userMain(argc, argv);

	// pre vracanja tajmera, da mogu da se probude zaostale niti, ne mogu to bez mog tajmera
	delete KernelSem::sleeping;
	LOCK
	SETVECT(8, PCB::oldTimerIntr);
	UNLOCK

	delete PCB::idle;
	PCB::running->state.setStateFinished();
	delete PCB::mainThread;
	delete PCB::allThreads;

	return result;
}
