#ifndef H_SLEEPLST_H
#define H_SLEEPLST_H

#include "thread.h"

class SleepingList {
public:
	SleepingList();
	~SleepingList();
	void add(PCB* pcb, Time timeToSleep);
	void remove(PCB* pcb);
	void timerTick();

private:
	struct SleepingElem {
		SleepingElem* next;
		PCB* pcb;
		Time timeToSleep;
		SleepingElem(PCB* p, Time t) {
			next = 0;
			pcb = p;
			timeToSleep = t;
		}
	};
	SleepingElem* head;
};

#endif /* H_SLEEPLST_H */
