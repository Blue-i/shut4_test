///*
// * ProjectorTests.cpp
// *
// *  Created on: 31 Oct 2016
// *      Author: rag
// */
//
#include "catch.h"
#include "Projector.h"
#include "Ethernet2.h"
#include "MockComponent.h"
#include "MockEventBus.h"
#include "Events.h"
#include <iostream>
#include <string.h>
#include "Logging.h"
using namespace std;


TEST_CASE("Projector starts in connecting state, and tries to connect every POLL_INTERVAL"){
//	Log.on();
	//given a proejctor
	char buffer[512] = "";
	MockEthernetClient c;
	MockEventBus ev;
	MockComponent s;
	IPAddress ip;
	Projector p(&c, &ip, 123, &ev, &s);
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//when the ethernet client is not connected and will not connect
	c.setConnectFlag(false);
	c.setConnectedFlag(false);

	//then the projector should remain in connectint state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//and should have called stop on the client
	REQUIRE(c.getStopFlag() == true);

	//and should have emitted no events
	REQUIRE(ev.getLastEvent() == NO_EVENT);

	//when run POLL_INTERVAL times
	for(int i = 0; i < PROJECTOR_POLL_INTERVAL; i++){
		p.run();
		//projector should remain in connecting state
		REQUIRE(p.getState() == Projector::STATE::CONNECTING);
		//and should not have been stopped
		REQUIRE(c.getStopFlag() == false);
	}

	//when run again
	p.run();

	//should remain in connecting state
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//and should have been stopped
	REQUIRE(c.getStopFlag() == true);
//	Log.off();
}

TEST_CASE("Projector stays in CONNECTING state when the device connects but does not respond"){
	//given a proejctor
	char buffer[512] = "";
	MockEthernetClient c;
	MockEventBus ev;
	MockComponent s;
	IPAddress ip;
	Projector p(&c, &ip, 123, &ev, &s);
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//when the ethernet client is not connected and will connect
	c.setConnectFlag(true);
	c.setConnectedFlag(false);

	//then the projector should stay in CONNECTING state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//and emit no events
	REQUIRE(ev.getLastEvent() == NO_EVENT);

	//and client should be stopped
	REQUIRE(c.getStopFlag() == true);

}

TEST_CASE("Projector goes from CONNECTING to CONNECTED with PROJECTOR_CONNECTED event"){
	//given a proejctor
	char buffer[512] = "";
	MockEthernetClient c;
	MockEventBus ev;
	MockComponent s;
	IPAddress ip;
	Projector p(&c, &ip, 123, &ev, &s);
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//when the ethernet client is not connected and will connect
	c.setConnectFlag(true);
	c.setConnectedFlag(false);

	//and the device will respond
	c.setReadBuffer("OK\r");

	//the projector should go to CONNECTED state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTED);

	//and the projector should emit PROJECTOR_CONNECTED
	REQUIRE(ev.getLastEvent() == PROJECTOR_CONNECTED);

	//and the client should not be stopped
	REQUIRE(c.getStopFlag() == false);
}

TEST_CASE("Projector goes from CONNECTED to POLLING every POLL_INTERVAL"){
	//given a proejctor
	char buffer[512] = "";
	MockEthernetClient c;
	MockEventBus ev;
	MockComponent s;
	IPAddress ip;
	Projector p(&c, &ip, 123, &ev, &s);
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//when the ethernet client is not connected and will connect
	c.setConnectFlag(true);
	c.setConnectedFlag(false);

	//and the device will respond
	c.setReadBuffer("OK\r");

	//the projector should go to CONNECTED state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTED);

	//and the projector should emit PROJECTOR_CONNECTED
	REQUIRE(ev.getLastEvent() == PROJECTOR_CONNECTED);

	//and the client should not be stopped
	REQUIRE(c.getStopFlag() == false);


	//when the projector is run POLL_INTERVAL times
	for(int i = 0; i < PROJECTOR_POLL_INTERVAL; i++){
		p.run();
		//projector should remain in CONNECTED state
		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
		//and shutter should be run
		REQUIRE(s.isRun());
	}

	//when the proejctor is run again
	p.run();

	//should go to POLLING state
	REQUIRE(p.getState() == Projector::STATE::POLLING);
}

TEST_CASE("Projector goes from POLLING to CONNECTED when device responds with OK"){
	//given a proejctor
	char buffer[512] = "";
	MockEthernetClient c;
	MockEventBus ev;
	MockComponent s;
	IPAddress ip;
	Projector p(&c, &ip, 123, &ev, &s);
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//when the ethernet client is not connected and will connect
	c.setConnectFlag(true);
	c.setConnectedFlag(false);

	//and the device will respond
	c.setReadBuffer("OK\r");

	//the projector should go to CONNECTED state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTED);

	//and the projector should emit PROJECTOR_CONNECTED
	REQUIRE(ev.getLastEvent() == PROJECTOR_CONNECTED);

	//and the client should not be stopped
	REQUIRE(c.getStopFlag() == false);


	//when the projector is run POLL_INTERVAL times
	for(int i = 0; i < PROJECTOR_POLL_INTERVAL; i++){
		p.run();
		//projector should remain in CONNECTED state
		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
		//and shutter should be run
		REQUIRE(s.isRun());
	}

	//when the proejctor is run again
	p.run();

	//should go to POLLING state
	REQUIRE(p.getState() == Projector::STATE::POLLING);



	//when device will respond with OK
	c.setReadBuffer("OK\r");

	//the projector will go to CONNECTED state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTED);

	//the projector should not emit any events
	REQUIRE(ev.getLastEvent() == NO_EVENT);
}

TEST_CASE("Projector goes from POLLING to CONNECTING if the device does not respond and emits PROJECTOR_DISCONNECTED event"){
	//given a proejctor
	char buffer[512] = "";
	MockEthernetClient c;
	MockEventBus ev;
	MockComponent s;
	IPAddress ip;
	Projector p(&c, &ip, 123, &ev, &s);
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//when the ethernet client is not connected and will connect
	c.setConnectFlag(true);
	c.setConnectedFlag(false);

	//and the device will respond
	c.setReadBuffer("OK\r");

	//the projector should go to CONNECTED state
	p.run();
	REQUIRE(p.getState() == Projector::STATE::CONNECTED);

	//and the projector should emit PROJECTOR_CONNECTED
	REQUIRE(ev.getLastEvent() == PROJECTOR_CONNECTED);

	//and the client should not be stopped
	REQUIRE(c.getStopFlag() == false);


	//when the projector is run POLL_INTERVAL times
	for(int i = 0; i < PROJECTOR_POLL_INTERVAL; i++){
		p.run();
		//projector should remain in CONNECTED state
		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
		//and shutter should be run
		REQUIRE(s.isRun());
	}

	//when the proejctor is run again
	p.run();

	//should go to POLLING state
	REQUIRE(p.getState() == Projector::STATE::POLLING);



	//when projector is run and the device does not respond
	p.run();

	//the projector should go to CONNECTING state
	REQUIRE(p.getState() == Projector::STATE::CONNECTING);

	//and emit PROJECTOR_DISCONNECTED
	REQUIRE(ev.getLastEvent() == PROJECTOR_DISCONNECTED);
}

//
//TEST_CASE("Projector Tests"){
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//readFor
//	m.setReadBuffer("READFOR\r");
//	size_t bytes = p.readFor(buffer, 512, 10);
//	REQUIRE(bytes == 7);
//	REQUIRE(strcmp(buffer, "READFOR") == 0);
//
//	m.setReadBuffer("0987654321");
//	bytes = p.readFor(buffer, 512, 10);
//	REQUIRE(bytes == 0);
//	REQUIRE(strcmp(buffer, "") == 0);
//
//	char smallBuffer[2] = "";
//	m.setReadBuffer("0987654321");
//	bytes = p.readFor(smallBuffer, 2, 10);
//	REQUIRE(bytes == 0);
//	REQUIRE(strcmp(buffer, "") == 0);
//}
//
//TEST_CASE("Projector connects and polls with good ethernetclient"){
//	//given a new projector and well behaved ethernet client
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//should start in connecting
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//	//when run
//	p.run();
//	//should go to connected
//	REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//	//when run more than poll interval
//	for(int i = 0; i < Projector::POLL_INTERVAL; i++){
//		p.run();
//		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//	}
//	p.run();
//	//should go to polling state
//	REQUIRE(p.getState() == Projector::STATE::POLLING);
//	//when run in polling state
//	m.setReadBuffer("Poll\r");
//	p.run();
//	//should print query message to client
//	REQUIRE(m.getPrintBuffer() == "%1AVMT ?\r");
//	//and go back to connected state
//	REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//}
//
//TEST_CASE("Projector remains in connecting state until ethernet client really connects"){
//	//given a new projector and ethernet client which can't connect
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when the client can't connect
//	m.setConnectFlag(false);
//	m.setConnectedFlag(false);
//
//	//should start in connecting
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//	p.run();
//	//and remain in connecting
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//
//	//when the client returns true for connect but isn't connected
//	m.setConnectFlag(true);
//
//	//should remain in connecting
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//
//	//when the client returns true for connected and connecting
//	m.setConnectedFlag(true);
//
//	//should go to connected
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//}
//
//TEST_CASE("Projector goes back to connecting state after poll timeout"){
//	//given a new projector and connnected client
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when the projected is in polling state
//	p.run();
//	for(int i = 0; i < Projector::POLL_INTERVAL; i++){
//		p.run();
//		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//	}
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::POLLING);
//
//	//when projector gets no poll response
//	m.setReadBuffer("");
//	p.run();
//
//	//client should be stopped
//	REQUIRE(m.getStopFlag());
//
//	//should print query message to client
//	REQUIRE(m.getPrintBuffer() == "%1AVMT ?\r");
//
//	//should be back in connecting state
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//}
//
//TEST_CASE("Projector goes back to connecting state after reset"){
//	//given a new projector and ethernet client
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when the projector is in connected state
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//
//	//when reset is called
//	p.reset();
//
//	//then client should be stopped
//	REQUIRE(m.getStopFlag());
//
//	//and projector should be in connecting state
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//}
//
//TEST_CASE("Projector writes to client in connected state only"){
//	//given a new projector and ethernet client
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when the projector is in connecting state
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//
//	//then projector should not wirte to client
//	p.write("MESSAGE");
//	REQUIRE(m.getPrintBuffer() == "");
//
//	//when the projector is in connected state
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//
//	//the projector can write to the client
//	p.write("MESSAGE");
//	REQUIRE(m.getPrintBuffer() == "MESSAGE");
//
//	//when the projector is in polling state
//	for(int i = 0; i < Projector::POLL_INTERVAL; i++){
//		p.run();
//		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//	}
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::POLLING);
//
//	//the projector should not be able to write to the client
//	p.write("MESSAGE");
//	REQUIRE(m.getPrintBuffer() == "");
//}
//
//
//
//TEST_CASE("Projector resets components when entering connecting state"){
//	//given a new projector and ethernet client and component
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockComponent c;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when adding a component
//	bool accepted = p.addComponent(&c);
//
//	//projector should accept the component
//	REQUIRE(accepted);
//
//	//when client wont connect (so projector enters connecting state again on a loop)
//	m.setConnectFlag(false);
//	m.setConnectedFlag(false);
//
//	//when client re-enters connecting state
//	p.reset();
//	REQUIRE(p.getState() == Projector::STATE::CONNECTING);
//
//	//compoonent should be reset
//	REQUIRE(c.isReset());
//}
//
//TEST_CASE("Projector emmits disconnect events on reset"){
//	//given a new projector client and event bus
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when reset
//	p.reset();
//
//	//then event bus should receive a disconnected event
//	REQUIRE(ev.getLastEvent() == PROJECTOR_DISCONNECTED);
//}
//
//TEST_CASE("Projector does not transition to connected state if read fails."){
//	//given a new projector client and event bus
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when client is not already connected
//	m.setConnectedFlag(false);
//
//	//projector calls stop on client and remains in connecting state
//	p.run();
//	REQUIRE(m.getStopFlag());
//	REQUIRE(p.getState() == Projector::CONNECTING);
//}
//
//TEST_CASE("Projector emmits connect and disconnect events only on connect/disconnect, not on poll"){
//	//given a new projector client and event bus
//	char buffer[512] = "";
//	MockEthernetClient m;
//	MockEventBus ev;
//	IPAddress ip;
//	Projector p(&m, &ip, 123, &ev);
//
//	//when client is not already connected
//	m.setConnectedFlag(false);
//
//	//and client read will succeed on connect
//	m.setReadBuffer("REPLY\r");
//	p.run();
//
//	//projector should have emmited connected event
//	REQUIRE(ev.getLastEvent() == PROJECTOR_CONNECTED);
//
//	//when the projector is in polling state
//	for(int i = 0; i < Projector::POLL_INTERVAL; i++){
//		p.run();
//		REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//	}
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::POLLING);
//
//	//should not emit connect event when going back to connected state
//	m.setReadBuffer("REPLY\r");
//	p.run();
//	p.run();
//	REQUIRE(p.getState() == Projector::STATE::CONNECTED);
//
//	//then event bus should not receive a disconnected event
//	REQUIRE(ev.getLastEvent() == NO_EVENT);
//}
