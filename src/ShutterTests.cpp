///*
// * ShutterTests.cpp
// *
// *  Created on: 1 Nov 2016
// *      Author: rag
// */
//
#include "catch.h"
#include "Shutter.h"
#include "MockDevice.h"
#include "Events.h"
#include "MockEventBus.h"

#include <iostream>

TEST_CASE("Shutter starts in UNKNOWN state"){
	//given a shutter
	MockEventBus ev;
	Shutter s(&ev);

	//should be in unknown state
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
}


TEST_CASE("Shutter remains in unknown state when device does not respond"){
	//given a shutter and mock device
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when shutter runs in unknown state with a device that does not respond
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then device should receive an AVMT query
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");

	//shutter should stay in unknown state
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
}

TEST_CASE("Shutter polls at poll interval in unknown state"){
	//given a shutter and mock device
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when run in unknown state
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//shutter writes and resets device
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");

	//shutter runs POLL_INTERVAL times without writing and reseting device
	for(int i = 0; i < SHUTTER_POLL_INTERVAL; i++){
		s.run();
		REQUIRE(d.getWritebuf() == "");

	}
	//then shutter writes query message on next run
	s.run();
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
}

TEST_CASE("Shutter goes from UNKNOWN to OPEN state and emits SHUTTER_OPENED when response is AVMT 30"){
	//given a shutter and mock device
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);
	d.setReadbuf("%1AVMT=30\r");

	//when run in unknown state
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then Shutter should go to open state
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and EventBus should get SHUTTER_OPENED event
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);
}

TEST_CASE("Shutter goes from UNKNOWN to CLOSED state and emits SHUTTER_CLOSED event when response is AVMT 31"){
	//given a shutter and mock device
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);
	d.setReadbuf("%1AVMT=31\r");

	//when run in unknown state
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then Shutter should go to open state
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and EventBus should get SHUTTER_OPENED event
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);
}

TEST_CASE("Shutter goes from OPEN to CLOSING on SHUTTER_CLOSE_BUTTON_PRESS event"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is OPEN
	d.setReadbuf("%1AVMT=30\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to open state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);

	//when shutter receives SHUTTER_CLOSE_BUTTON_PRESS event
	s.onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
	s.run();

	//the shutter should go to closing state and the device should comply
	d.setReadbuf("%1AVMT=OK\r");
	REQUIRE(s.getState() == Shutter::STATE::CLOSING);
}

TEST_CASE("Shutter goes from OPEN to CLOSED and emit SHUTTER_CLOSED when the device response with AVMT 31"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is OPEN
	d.setReadbuf("%1AVMT=30\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to open state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);

	//when the shutter is ready to poll
	for(int i = 0; i < SHUTTER_POLL_INTERVAL; i++){
		s.run();
		REQUIRE(s.getState() == Shutter::STATE::OPEN);
	}

	//and the device response with AVMT 31
	d.setReadbuf("%1AVMT=31\r");

	//then the device should go to CLOSED state
	s.run();
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit the SHUTTER_CLOSED event
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);
}

TEST_CASE("Shutter stays in OPEN state when response is AVMT 30"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is OPEN
	d.setReadbuf("%1AVMT=30\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to open state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);

	//when the shutter is ready to poll
	for(int i = 0; i < SHUTTER_POLL_INTERVAL; i++){
		s.run();
		REQUIRE(s.getState() == Shutter::STATE::OPEN);
	}

	//and the device response with AVMT 30
	d.setReadbuf("%1AVMT=30\r");

	//then the device should remain in OPEN state
	s.run();
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit the no events
	REQUIRE(ev.getLastEvent() == NO_EVENT);
}

TEST_CASE("Shutter goes from CLOSED to OPENING on SHUTTER_OPEN_BUTTON_PRESS event"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is CLOSED
	d.setReadbuf("%1AVMT=31\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to closed state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit SHUTTER_CLOSED
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);

	//when shutter receives SHUTTER_OPEN_BUTTON_PRESS event
	s.onEvent(SHUTTER_OPEN_BUTTON_PRESS);
	s.run();

	//the shutter should go to closing state and the device should comply
	d.setReadbuf("%1AVMT=OK\r");
	REQUIRE(s.getState() == Shutter::STATE::OPENING);
}

TEST_CASE("Shutter goes from CLOSED to OPEN and emit SHUTTER_OPENED when the device response with AVMT 30"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is CLOSED
	d.setReadbuf("%1AVMT=31\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to closed state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit SHUTTER_CLOSED
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);

	//when the shutter is ready to poll
	for(int i = 0; i < SHUTTER_POLL_INTERVAL; i++){
		s.run();
		REQUIRE(s.getState() == Shutter::STATE::CLOSED);
	}

	//and the device response with AVMT 30
	d.setReadbuf("%1AVMT=30\r");

	//then the device should go to OPEN state
	s.run();
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit the SHUTTER_OPENED event
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);
}

TEST_CASE("Shutter stays in CLOSED state when response is AVMT 31"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is CLOSED
	d.setReadbuf("%1AVMT=31\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to closed state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);

	//when the shutter is ready to poll
	for(int i = 0; i < SHUTTER_POLL_INTERVAL; i++){
		s.run();
		REQUIRE(s.getState() == Shutter::STATE::CLOSED);
	}

	//and the device response with AVMT 30
	d.setReadbuf("%1AVMT=31\r");

	//then the device should remain in CLOSED state
	s.run();
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit the no events
	REQUIRE(ev.getLastEvent() == NO_EVENT);
}

TEST_CASE("Shutter goes from OPENING to OPEN when response is OK"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is CLOSED
	d.setReadbuf("%1AVMT=31\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to closed state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);

	//when shutter receives SHUTTER_OPEN_BUTTON_PRESS event
	s.onEvent(SHUTTER_OPEN_BUTTON_PRESS);
	s.run();

	//the shutter should go to OPENING state
	REQUIRE(s.getState() == Shutter::STATE::OPENING);

	//and the device responds with OK
	d.setReadbuf("%1AVMT=OK\r");

	//the shutter should go to OPEN state
	s.run();
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit event SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);
}

TEST_CASE("Shutter should go from CLOSING to CLOSED when the device responds with OK"){
	//given a shutter in UNKNOWN state
	char buffer[32];
	MockDevice d;
	MockEventBus ev;
	Shutter s(&ev);
	s.bind(&d, buffer, 32);

	//when the response is OPEN
	d.setReadbuf("%1AVMT=30\r");
	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
	s.run();

	//then the shutter goes to open state after writing the query message
	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
	REQUIRE(s.getState() == Shutter::STATE::OPEN);

	//and emit SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_OPENED);

	//when shutter receives SHUTTER_CLOSE_BUTTON_PRESS event
	s.onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
	s.run();

	//the shutter should go to closing state
	REQUIRE(s.getState() == Shutter::STATE::CLOSING);

	//the device should respond with OK
	d.setReadbuf("%1AVMT=OK\r");

	//the shutter should go to CLOSED state
	s.run();
	REQUIRE(s.getState() == Shutter::STATE::CLOSED);

	//and emit event SHUTTER_OPENED
	REQUIRE(ev.getLastEvent() == SHUTTER_CLOSED);
}

//TEST_CASE("Shutter goes from OPEN to CLOSING to UNKNOWN on SHUTTER_CLOSE_BUTTON_PRESS event"){
//	//given a shutter in OPEN state and mock device
//	char buffer[32];
//	MockDevice d;
//	MockEventBus ev;
//	Shutter s(&ev);
//	s.bind(&d, buffer, 32);
//	d.setReadbuf("%1AVMT=30\r");
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	REQUIRE(s.getState() == Shutter::STATE::OPEN);
//
//	//when shutter receives SHUTTER_CLOSE_BUTTON_PRESS event
//	s.onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
//	s.run();
//
//	//the shutter should go to closing state
//	REQUIRE(s.getState() == Shutter::STATE::CLOSING);
//	s.run();
//
//	//then shutter should have written close message to device
//	REQUIRE(d.getWritebuf() == "%1AVMT 31\r");
//
//	//then shutter should go to closed state
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//}
//
//TEST_CASE("Shutter goes from OPEN to CLOSED after polling returns closed state"){
//	//given a shutter in OPEN state and mock device
//	char buffer[32];
//	MockDevice d;
//	MockEventBus ev;
//	Shutter s(&ev);
//	s.bind(&d, buffer, 32);
//	d.setReadbuf("%1AVMT=30\r");
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	REQUIRE(s.getState() == Shutter::STATE::OPEN);
//
//	//when poll interval expires
//	for(int i = 0; i < Shutter::POLL_INTERVAL; i++){
//		s.run();
//		REQUIRE(s.getState() == Shutter::STATE::OPEN);
//	}
//	d.setReadbuf("%1AVMT=31\r");
//
//	//shutter will write query message to device
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	//and go to CLOSED state
//	REQUIRE(s.getState() == Shutter::STATE::CLOSED);
//}
//
//
//
//
//
//TEST_CASE("Shutter goes from CLOSED to OPENING to OPEN on SHUTTER_OPEN_BUTTON_PRESS event"){
//	//given a shutter in CLOSED state and mock device
//	char buffer[32];
//	MockDevice d;
//	MockEventBus ev;
//	Shutter s(&ev);
//	s.bind(&d, buffer, 32);
//	d.setReadbuf("%1AVMT=31\r");
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	REQUIRE(s.getState() == Shutter::STATE::CLOSED);
//
//	//when shutter receives SHUTTER_OPEN_BUTTON_PRESS event
//	s.onEvent(SHUTTER_OPEN_BUTTON_PRESS);
//	s.run();
//
//	//the shutter should go to opening state and the device should comply
//	d.setReadbuf("%1AVMT=OK\r");
//	REQUIRE(s.getState() == Shutter::STATE::OPENING);
//	s.run();
//
//	//then shutter should have written open message to device
//	REQUIRE(d.getWritebuf() == "%1AVMT 30\r");
//
//	//then shutter should go to closed state
//	REQUIRE(s.getState() == Shutter::STATE::OPEN);
//}
//
//TEST_CASE("Shutter goes from CLOSED to OPENING to UNKNOWN on SHUTTER_CLOSE_BUTTON_PRESS event"){
//	//given a shutter in CLOSED state and mock device
//	char buffer[32];
//	MockDevice d;
//	MockEventBus ev;
//	Shutter s(&ev);
//	s.bind(&d, buffer, 32);
//	d.setReadbuf("%1AVMT=31\r");
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	REQUIRE(s.getState() == Shutter::STATE::CLOSED);
//
//	//when shutter receives SHUTTER_OPEN_BUTTON_PRESS event
//	s.onEvent(SHUTTER_OPEN_BUTTON_PRESS);
//	s.run();
//
//	//the shutter should go to OPENING state
//	REQUIRE(s.getState() == Shutter::STATE::OPENING);
//	s.run();
//
//	//then shutter should have written open message to device
//	REQUIRE(d.getWritebuf() == "%1AVMT 30\r");
//
//	//then shutter should go to closed state
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//}
////
//TEST_CASE("Shutter goes from CLOSED to OPEN after polling returns open state"){
//	//given a shutter in CLOSED state and mock device
//	char buffer[32];
//	MockDevice d;
//	MockEventBus ev;
//	Shutter s(&ev);
//	s.bind(&d, buffer, 32);
//	d.setReadbuf("%1AVMT=31\r");
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	REQUIRE(s.getState() == Shutter::STATE::CLOSED);
//
//	//when poll interval expires
//	for(int i = 0; i < Shutter::POLL_INTERVAL; i++){
//		s.run();
//		REQUIRE(s.getState() == Shutter::STATE::CLOSED);
//	}
//	d.setReadbuf("%1AVMT=30\r");
//
//	//shutter will write query message to device
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	//and go to CLOSED state
//	REQUIRE(s.getState() == Shutter::STATE::OPEN);
//}
//
//
//TEST_CASE("Shutter goes to UNKNOWN on reset"){
//	//given a shutter in CLOSED state and mock device
//	char buffer[32];
//	MockDevice d;
//	MockEventBus ev;
//	Shutter s(&ev);
//	s.bind(&d, buffer, 32);
//	d.setReadbuf("%1AVMT=31\r");
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//	s.run();
//	REQUIRE(d.getWritebuf() == "%1AVMT ?\r");
//	REQUIRE(s.getState() == Shutter::STATE::CLOSED);
//
//	//when reset
//	s.reset();
//
//	//then shutter should be in UNKNOWN state
//	REQUIRE(s.getState() == Shutter::STATE::UNKNOWN);
//}
