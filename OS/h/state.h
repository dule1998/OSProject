#ifndef H_STATE_H_
#define H_STATE_H_

class State {
	int state;
public:
	State();
	~State();

	void setStateReady();
	void setStateBlocked();
	void setStateFinished();

	int getState();

	static int Blocked;
	static int Ready;
	static int Finished;
	static int New;
};



#endif /* H_STATE_H_ */
