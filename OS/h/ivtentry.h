#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

typedef void interrupt (*pInterrupt)(...);

#include "thread.h"
#include "kernev.h"


class IVTEntry {
public:
	IVTEntry(IVTNo num, pInterrupt myIntr);
	~IVTEntry();

	static IVTEntry* ivtEntries[256];
	IVTNo num;
	KernelEv* event; // dogadjaj registrovan da mu se poziva signal() kad dodje prekid
	pInterrupt oldIntr; // prethodna prekidna rutina
};

#define PREPAREENTRY(ivtNum, flagOldIntr) \
void interrupt intr##ivtNum(...); \
IVTEntry ivtEntry##ivtNum(ivtNum, &intr##ivtNum); \
void interrupt intr##ivtNum(...) { \
	if (flagOldIntr) { \
		if (ivtEntry##ivtNum.oldIntr) { \
			(*ivtEntry##ivtNum.oldIntr)(); \
		} \
	} \
	if (ivtEntry##ivtNum.event) { \
		ivtEntry##ivtNum.event->signal(); \
	} \
	dispatch(); \
}

#endif /* H_IVTENTRY_H_ */
