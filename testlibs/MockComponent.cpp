/*
 * MockComponent.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#include <MockComponent.h>

MockComponent::MockComponent() : device(0), resetFlag(false), runFlag(false) {
	// TODO Auto-generated constructor stub

}

MockComponent::~MockComponent() {
	// TODO Auto-generated destructor stub
}

void MockComponent::bind(Device* d, char*, size_t) {
	device = d;
}

bool MockComponent::isBound() {
	return device != nullptr;
}

void MockComponent::run() {
	runFlag = true;
}

void MockComponent::reset() {
	resetFlag = true;
}

bool MockComponent::isReset() {
	bool b = resetFlag;
	if(resetFlag) resetFlag = false;
	return b;
}

bool MockComponent::isRun() {
	bool f = runFlag;
	runFlag = false;
	return f;
}
