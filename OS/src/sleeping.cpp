#include "pcb.h"
#include "kernsem.h"
#include "schedule.h"
#include <assert.h>
#include "sleeping.h"


SleepingList::SleepingList() {
	head = 0;
}

SleepingList::~SleepingList() {
	while (head)
		timerTick();
}

void SleepingList::add(PCB* pcb, Time timeToSleep) {
	if ((!pcb) || (timeToSleep == 0))
		return;

	SleepingElem *cur = head, *prev = 0;

	while (cur && cur->timeToSleep <= timeToSleep) {
		timeToSleep -= cur->timeToSleep;
		prev = cur;
		cur = cur->next;
	}

	SleepingElem* elem = new SleepingElem(pcb, timeToSleep);
	if (!prev)
		head = elem;
	else
		prev->next = elem;

	elem->next = cur;
	if (cur)
		cur->timeToSleep -= elem->timeToSleep;
}

void SleepingList::remove(PCB* pcb) {
	if (!head || !pcb)
		return;

	SleepingElem *elem = head, *prev = 0;
	while (elem && elem->pcb != pcb) {
		prev = elem;
		elem = elem->next;
	}

	if (elem) {
		if (!prev)
			head = elem->next;
		else
			prev->next = elem->next;

		if (elem->next)
			elem->next->timeToSleep += elem->timeToSleep;


		delete elem;
	}
}

void SleepingList::timerTick() {
	if (head)
		head->timeToSleep--;

	while (head  && (head->timeToSleep == 0)) {
		SleepingElem* old = head;
		head = head->next;
		old->pcb->state.setStateReady();
		Scheduler::put(old->pcb);
		old->pcb->semReturn = 0;
		old->pcb->ksem->remove(old->pcb);
		old->pcb->ksem = 0;
		delete old;
	}
}

