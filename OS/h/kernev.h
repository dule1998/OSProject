#ifndef H_KERNEV_H_
#define H_KERNEV_H_

class PCB;
class IVTEntry;
typedef unsigned char IVTNo;


class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();
	virtual void wait();
	virtual void signal();

private:
	IVTNo ivtNo;
	IVTEntry* ivtEntry;
	int v;
	PCB* myPCB;
};


#endif /* H_KERNEV_H_ */
