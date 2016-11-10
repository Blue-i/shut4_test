/*
 * MockEventBus.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#include <MockEventBus.h>

MockEventBus::MockEventBus() {
	// TODO Auto-generated constructor stub

}

MockEventBus::~MockEventBus() {
	// TODO Auto-generated destructor stub
}

void MockEventBus::queueEvent(Event e) {
	lastEvent = e;
}

void MockEventBus::addHandler(EventHandler*) {
}

Event MockEventBus::getLastEvent() {
	Event e = lastEvent;
	lastEvent = NO_EVENT;
	return e;
}
