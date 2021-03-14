
#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_


typedef unsigned int Time;
class SleepingList;
class PCBQueue;
class PCB;

class KernelSem {
public:
	KernelSem(int init);
	virtual ~KernelSem();
	virtual int signal(int n);
	virtual int wait(Time maxTimeToWait);
	virtual void remove(PCB* pcb); // probudi bas ovu nit ako moze, ako ne moze, onda nista
	int getVal();

private:
	PCBQueue* blocked; // niti blokirane na semaforu
	int v; // vrednost semafora

	friend int main(int argc, char* argv[]); // da moze da dohvati sleeping listu
	friend class PCB; // da moze da dohvati sleeping listu
	static SleepingList* sleeping;
};


#endif /* H_KERNSEM_H_ */
