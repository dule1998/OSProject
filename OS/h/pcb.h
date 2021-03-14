#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"
#include "state.h"

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class Idle;
class KernelSem;
class SleepList;
class PCBQueue;

class PCB {
public:
	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	virtual ~PCB();
	void start();
	void initStack();
	void waitToComplete();
	static void interrupt dispatchImpl();
	static void interrupt timerIntr(...);
	static void runWrapper(Thread* thread);
	ID getId();
	static Thread* getThreadById(ID id);

	static pInterrupt oldTimerIntr;
	static Time runCounter;
	static Thread* mainThread;
	static PCB* running;
	static PCB* idlePCB;
	static Idle* idle;
	static PCBQueue* allThreads;

	static ID nextId;
	ID id;
	unsigned int ss, sp, bp;
	unsigned int* stack;
	StackSize stackSize;
	Time timeSlice;
	Thread* myThread;

	State state;

	PCBQueue* blockedThreads;

	int semReturn;
	KernelSem* ksem;
};


#endif /* H_PCB_H_ */
