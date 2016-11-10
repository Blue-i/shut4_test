/*
 * MockEventHandler.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#include <MockEventHandler.h>

MockEventHandler::MockEventHandler() : lastEvent(NO_EVENT){
	// TODO Auto-generated constructor stub

}

MockEventHandler::~MockEventHandler() {
	// TODO Auto-generated destructor stub
}

void MockEventHandler::onEvent(Event e) {
	lastEvent = e;
}

Event MockEventHandler::getLastEvent() {
	Event e = lastEvent;
	lastEvent = NO_EVENT;
	return e;
}
