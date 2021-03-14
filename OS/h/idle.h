
#ifndef H_IDLETHR_H_
#define H_IDLETHR_H_


#include "thread.h"

class Idle : public Thread {
public:
	void run();
	Idle();
	virtual ~Idle();

private:
	int keepRunning;
};


#endif /* H_IDLETHR_H_ */
