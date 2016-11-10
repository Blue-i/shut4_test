/*
 * MockEventHandler.h
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#ifndef MOCKEVENTHANDLER_H_
#define MOCKEVENTHANDLER_H_

#include "EventHandler.h"
#include "Events.h"

class MockEventHandler : public EventHandler{

	Event lastEvent;

public:
	MockEventHandler();
	virtual ~MockEventHandler();

	void onEvent(Event);

	Event getLastEvent();
};

#endif /* MOCKEVENTHANDLER_H_ */
