#include "ivtentry.h"
#include "pcb.h"
#include "macros.h"

IVTEntry* IVTEntry::ivtEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo num, pInterrupt myIntr) {
	LOCK
	oldIntr = GETVECT(num);
	SETVECT(num, myIntr);
	this->num = num;
	event = 0;
	ivtEntries[num] = this;
	UNLOCK
}

IVTEntry::~IVTEntry() {
	LOCK
	if (oldIntr) {
		(*oldIntr)();
	}
	SETVECT(num, oldIntr);
	ivtEntries[num] = 0;
	UNLOCK
}

