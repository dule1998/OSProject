#include "pcbqueue.h"
#include "pcb.h"


PCBQueue::PCBQueue() {
	head = tail = 0;
}

PCBQueue::~PCBQueue() {
	while (head)
		takeHead();
}

void PCBQueue::add(PCB* pcb) {
	Elem* elem = new Elem(pcb);
	if (head == 0) {
		head = elem;
		tail = head;
	}
	else {
		tail->next = elem;
		tail = tail->next;
	}
}

PCB* PCBQueue::takeHead() {
	if (!head)
		return 0;

	Elem* elem = head;
	head = head->next;
	PCB* pcb = elem->pcb;
	delete elem;
	if (head == 0)
		tail = 0;
	return pcb;
}

void PCBQueue::remove(PCB* pcb) {
	Elem *elem = head, *prev = 0;
	while (elem && (elem->pcb != pcb)) {
		prev = elem;
		elem = elem->next;
	}

	if (elem) {
		if (!prev)
			head = elem->next;
		else
			prev->next = elem->next;

		if (!elem->next)
			tail = prev;

		delete elem;
	}
}

PCB* PCBQueue::find(ID id) {
	Elem* elem = head;
	while (elem && elem->pcb->id != id)
		elem = elem->next;


	if (!elem)
		return 0;
	else
		return elem->pcb;
}

