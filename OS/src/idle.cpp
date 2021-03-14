#include "idle.h"


void Idle::run() {
	while (keepRunning)
		dispatch();
}


Idle::Idle() : Thread(4096, 1) {
	keepRunning = 1;
}

Idle::~Idle() {
	keepRunning = 0;
	waitToComplete();
}



