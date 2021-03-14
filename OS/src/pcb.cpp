#include "pcb.h"
#include "macros.h"
#include "state.h"
#include "schedule.h"
#include "kernsem.h"
#include <assert.h>
#include <iostream.h>
#include "idle.h"
#include "pcbqueue.h"
#include <stdlib.h>
#include "sleeping.h"

void _FAR * _FARFUNC operator new(unsigned size) {
#ifndef BCC_BLOCK_IGNORE
	asm { pushf; cli; }
#endif
	void _FAR * membuf = malloc(size);
#ifndef BCC_BLOCK_IGNORE
	asm { popf; }
#endif
	return membuf;
}

void _FARFUNC operator delete(void _FAR * p) {
#ifndef BCC_BLOCK_IGNORE
	asm { pushf; cli; }
#endif
	free(p);
#ifndef BCC_BLOCK_IGNORE
	asm { popf; }
#endif
}

void tick();

unsigned int tsp = 0, tss = 0, tbp = 0;

PCB* PCB::running = 0;
Thread* PCB::mainThread = 0;
pInterrupt PCB::oldTimerIntr = 0;
Time PCB::runCounter = 0;
Idle* PCB::idle = 0;
PCB* PCB::idlePCB = 0;
PCBQueue* PCB::allThreads = 0;
ID PCB::nextId = 1;



PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) {
	allThreads->add(this);
	if (idlePCB == 0)
		idlePCB = this;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	myThread = thread;
	state = State();
	stack = 0;
	ss = sp = bp = 0;
	semReturn = 0;
	ksem = 0;
	blockedThreads = new PCBQueue();
	id = nextId++;
}

PCB::~PCB() {
	delete blockedThreads;
	delete[] stack;
	allThreads->remove(this);
}

void PCB::start() {
	if (state.getState() == State::New) {
		initStack();
		state.setStateReady();
		if (PCB::idlePCB != this)
			Scheduler::put(this);
	}
}

void PCB::waitToComplete() {
	if ((state.getState() == State::Ready || state.getState() == State::Blocked) && (running != this)) {
		blockedThreads->add(running);
		running->state.setStateBlocked();
		dispatch();
	}
}

Thread* PCB::getThreadById(ID id) {
	PCB* pcb = allThreads->find(id);
	if (pcb)
		return pcb->myThread;
	return 0;
}

ID PCB::getId() {
	return id;
}

void PCB::initStack() {
	StackSize size = stackSize / 2;
	stack = new unsigned[size];
	unsigned* top = stack + size - 1;
	*top = FP_SEG(myThread); top--;
	*top = FP_OFF(myThread); top--;
	*top = 0; top--;
	*top = 0; top--;
	*top = 512; top--;
	*top = FP_SEG(&runWrapper); top--;
	*top = FP_OFF(&runWrapper); top--;

	top = stack + size - 16;
	ss = FP_SEG(top);
	sp = FP_OFF(top);
	bp = FP_OFF(top);
}

void interrupt PCB::dispatchImpl() {
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}
#endif
	running->ss = tss;
	running->sp = tsp;
	running->bp = tbp;

	if (running->state.getState() == State::Ready && running != PCB::idlePCB)
		Scheduler::put(running);
	running = Scheduler::get();
	if (!running)
		running = PCB::idlePCB;

	if (!running)
		// ispisi poruku o gresci
		cout << endl << "running je 0" << endl;
	assert(running);

	runCounter = 0;

	tss = running->ss;
	tsp = running->sp;
	tbp = running->bp;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
#endif
}

void interrupt PCB::timerIntr(...) {
	(*oldTimerIntr)();
	tick();
	runCounter++;
	KernelSem::sleeping->timerTick();

	if (runCounter >= running->timeSlice) {
		if (running->timeSlice > 0) {
			dispatch();
		}
	}
}

void PCB::runWrapper(Thread* thread) {
//	running->myThread->run();
	thread->run();

	// gasi nit
	LOCK
	running->state.setStateFinished();
	while (running->blockedThreads->head) {
		PCB* pcb = running->blockedThreads->takeHead();
		Scheduler::put(pcb);
		pcb->state.setStateReady();
	}
	UNLOCK
	dispatch();
}

