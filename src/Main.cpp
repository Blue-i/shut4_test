/*
 * Main.cpp
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#define CATCH_CONFIG_MAIN
#include "Timer.h"
#include "PJlinkParser.h"
#include <iostream>
#include <assert.h>
#include "catch.h"
#include "Ethernet2.h"
//#include "RingBuffer.h"
#include "RingBufferFixed.h"
#include "Train.h"
#include "MockComponent.h"

using namespace std;


TEST_CASE("Timer Tests"){

	Timer t(4);

	REQUIRE(!t.completed());
	REQUIRE(!t.completed());
	REQUIRE(!t.completed());
	REQUIRE(!t.completed());
	REQUIRE(t.completed());

	t.reset();

	REQUIRE(!t.completed());
	REQUIRE(!t.completed());
	REQUIRE(!t.completed());
	REQUIRE(!t.completed());
	REQUIRE(t.completed());

	t.reset();
	t.expire();

	REQUIRE(t.completed());

}

TEST_CASE("PJLinkParser Tests"){

	PJLinkParser * p = PJLinkParser::instance();
	REQUIRE(p->parseMessage("X1AVMT=30") == PJLinkParser::UNKNOWN);
	REQUIRE(p->parseMessage("%0AVMT=30") == PJLinkParser::UNKNOWN);
	REQUIRE(p->parseMessage("%1AVMT-30") == PJLinkParser::UNKNOWN);
	REQUIRE(p->parseMessage("%1AVMT=OX") == PJLinkParser::UNKNOWN);
	REQUIRE(p->parseMessage("%1AVMT=OK") == PJLinkParser::OK);
	REQUIRE(p->parseMessage("%1AVMT=EX") == PJLinkParser::ERROR);
	REQUIRE(p->parseMessage("%1AVMT=EXX") == PJLinkParser::ERROR);
	REQUIRE(p->parseMessage("%1AVMT=EXXX") == PJLinkParser::ERROR);
	REQUIRE(p->parseMessage("%1AVMT=X0") == PJLinkParser::AVMT_OFF);
	REQUIRE(p->parseMessage("%1AVMT=X1") == PJLinkParser::AVMT_ON);
	REQUIRE(p->parseMessage("%1XXXX=X0") == PJLinkParser::UNKNOWN);
	REQUIRE(p->parseMessage("%1XXXX=X1") == PJLinkParser::UNKNOWN);
}

TEST_CASE("Mock Ethernet Client") {
	IPAddress ip;
	MockEthernetClient m;
	//stop
	m.stop();
	REQUIRE(m.getStopFlag());
	m.setStopFlag(true);

	//read
	REQUIRE(m.read() == -1);

	m.setReadBuffer("01234");

	for(int i = 0; i < 5; i++){
		REQUIRE(m.read() == (char)(48 + i));
	}

	REQUIRE(m.read() == -1);

	//connect connected
	REQUIRE(m.connect(ip, 123));
	REQUIRE(m.connected());

	m.setConnectFlag(false);

	REQUIRE(!m.connect(ip, 123));
	m.setConnectedFlag(false);
	REQUIRE(!m.connected());

}

//TEST_CASE("Ring buffer"){
//	RingBuffer<int> rb(4);
//
//	REQUIRE(rb.isEmpty());
//
//	REQUIRE(rb.enqueue(1));
//	REQUIRE(rb.enqueue(2));
//	REQUIRE(rb.enqueue(3));
//	REQUIRE(rb.enqueue(4));
//	REQUIRE(!rb.enqueue(5));
//	REQUIRE(rb.isFull());
//	REQUIRE(rb.dequeue() == 1);
//	REQUIRE(rb.dequeue() == 2);
//	REQUIRE(rb.dequeue() == 3);
//	REQUIRE(rb.dequeue() == 4);
//	REQUIRE(rb.isEmpty());
//
//	REQUIRE(rb.enqueue(1));
//}


//TEST_CASE("Ring buffer Fixed"){
//	RingBufferF<int, 4> rb;
//
//	REQUIRE(rb.isEmpty());
//
//	REQUIRE(rb.enqueue(1));
//	REQUIRE(rb.enqueue(2));
//	REQUIRE(rb.enqueue(3));
//	REQUIRE(rb.enqueue(4));
//	REQUIRE(!rb.enqueue(5));
//	REQUIRE(rb.isFull());
//	REQUIRE(!rb.isEmpty());
//	REQUIRE(rb.dequeue() == 1);
//	REQUIRE(rb.dequeue() == 2);
//	REQUIRE(rb.dequeue() == 3);
//	REQUIRE(rb.dequeue() == 4);
//	REQUIRE(rb.isEmpty());
//
//	REQUIRE(rb.enqueue(1));
//}


TEST_CASE("Train tests"){
	cout << "STARTING" << endl;
	Train t;
	char buffer[512] = "";
	MockComponent mc;
	Projector p1(nullptr, nullptr, 123, nullptr, &mc);
	Projector p2(nullptr, nullptr, 123, nullptr, &mc);
	Projector p3(nullptr, nullptr, 123, nullptr, &mc);
	Projector p4(nullptr, nullptr, 123, nullptr, &mc);
	Projector p5(nullptr, nullptr, 123, nullptr, &mc);

	cout << "FIRST" << endl;
	REQUIRE(t.dequeue() == nullptr);

	Projector * pp1 = &p1;
	Projector * pp2 = &p2;
	Projector * pp3 = &p3;
	Projector * pp4 = &p4;
	Projector * pp5 = &p5;

	t.enqueue(pp1);
	REQUIRE(t.dequeue() == pp1);
	t.enqueue(pp1);
	t.enqueue(pp2);
	t.enqueue(pp3);
	t.enqueue(pp4);
	t.enqueue(pp5);

	cout << "ENQUEUED" << endl;
	REQUIRE(t.dequeue() == pp1);
	cout << "DEQUED 1" << endl;
	REQUIRE(t.dequeue() == pp2);
	cout << "DEQUED 2" << endl;
	REQUIRE(t.dequeue() == pp3);
	cout << "DEQUED 3" << endl;
	REQUIRE(t.dequeue() == pp4);
	cout << "DEQUED 4" << endl;
	REQUIRE(!t.dequeue());
	cout << "DEQUED 5" << endl;

	t.enqueue(pp1);
	REQUIRE(t.dequeue() == pp1);
	t.enqueue(pp1);
	REQUIRE(t.dequeue() == pp1);
	t.enqueue(pp2);
	REQUIRE(t.dequeue() == pp2);
	t.enqueue(pp3);
	REQUIRE(t.dequeue() == pp3);
	t.enqueue(pp4);
	REQUIRE(t.dequeue() == pp4);
	t.enqueue(pp5);
	REQUIRE(t.dequeue() == pp5);
	t.enqueue(pp5);
	REQUIRE(t.dequeue() == pp5);
}
