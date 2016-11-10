/*
 * EventManagerTests.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#include "catch.h"
#include "MockEventHandler.h"
#include "EventManager.h"
#include "Events.h"

TEST_CASE("Event Manager should send queued event to all event handlers"){
	//given an EventManager and handlers
	EventManager em;
	MockEventHandler m1;
	MockEventHandler m2;
	em.addHandler(&m1);
	em.addHandler(&m2);

	//when an event is queued
	em.queueEvent(SHUTTER_OPENED);
	em.run();

	//both handlers should get the event
	REQUIRE(m1.getLastEvent() == SHUTTER_OPENED);
	REQUIRE(m2.getLastEvent() == SHUTTER_OPENED);
}


TEST_CASE("EventManager should send external events to all event handlers"){
	//given an EventManager and handlers
	EventManager em;
	MockEventHandler m1;
	MockEventHandler m2;
	em.addHandler(&m1);
	em.addHandler(&m2);

	//when an external event is signalled
	em.externalEvent = SHUTTER_OPENED;
	em.externalEventUpdate = true;
	em.run();

	//then both handlers should receive the event
	REQUIRE(m1.getLastEvent() == SHUTTER_OPENED);
	REQUIRE(m2.getLastEvent() == SHUTTER_OPENED);

	//and the external event variables should be reset
	REQUIRE(em.externalEventUpdate == false);
}

