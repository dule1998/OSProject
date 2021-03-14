#ifndef H_PCBQUEUE_H_
#define H_PCBQUEUE_H_

class PCB;
typedef int ID;

class PCBQueue {
public:
	PCBQueue();
	// po zelji dodaj virtual na dtor
	~PCBQueue();

	void add(PCB* pcb);
	PCB* takeHead();
	void remove(PCB* pcb);
	PCB* find(ID id);

	struct Elem {
		Elem* next;
		PCB* pcb;
		Elem(PCB* pcb) {
			next = 0;
			this->pcb = pcb;
		}
	};
	Elem *head, *tail;
};


#endif /* H_PCBQUEUE_H_ */
