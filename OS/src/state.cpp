#include "state.h"

int State::Finished = 3;
int State::Blocked = 2;
int State::Ready = 1;
int State::New = 0;

State::State() {
	state = 0;
}

State::~State() {

}

void State::setStateReady() {
	state = 1;
}

void State::setStateBlocked() {
	state = 2;
}

void State::setStateFinished() {
	state = 3;
}

int State::getState() {
	return state;
}

