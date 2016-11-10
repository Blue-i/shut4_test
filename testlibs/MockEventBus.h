/*
 * MockEventBus.h
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#ifndef MOCKEVENTBUS_H_
#define MOCKEVENTBUS_H_

#include "EventBus.h"
//#include "RingBuffer.h"

class MockEventBus : public EventBus {
	Event lastEvent = NO_EVENT;

public:
	MockEventBus();
	virtual ~MockEventBus();

	virtual void queueEvent(Event);
	virtual void addHandler(EventHandler*);

	Event getLastEvent();
};

#endif /* MOCKEVENTBUS_H_ */
