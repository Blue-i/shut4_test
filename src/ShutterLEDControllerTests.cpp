// * ShutterLEDControllerTests.cpp
// *
// *  Created on: 1 Nov 2016
// *      Author: rag
// */
//
//#include "catch.h"
//#include "ShutterLEDController.h"
//#include "Arduino.h"
//
//int redpin = 11;
//int greenpin = 10;
//
//TEST_CASE("Controller sets green and red pin to HIGH on construction"){
//	MockPins::instance()->clear();
//
//	//given a new ShutterLEDController
//	ShutterLEDController c(greenpin,redpin);
//
//	//when construncted
//
//	//greenpin should be high
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == HIGH);
//
//	//redpin should be high
//	REQUIRE(MockPins::instance()->getPinState(redpin) == HIGH);
//	MockPins::instance()->clear();
//}
//
//TEST_CASE("Controller should go in to OPENING state"){
//	MockPins::instance()->clear();
//
//	//given a new ShutterLEDController
//	ShutterLEDController c(greenpin,redpin);
//
//	//on OPEN_BUTTON and CONNECT
//	c.onEvent(SHUTTER_OPEN_BUTTON_PRESS);
//	c.onEvent(PROJECTOR_CONNECTED);
//	c.run();
//
//	//should be in OPENNG state
//	REQUIRE(c.getState() == ShutterLEDController::OPENING);
//
//	//green pin should be HIGH and red pin should be LOW
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == HIGH);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//
//	//green pin should be LOW after BLINK_INTERVAL iterations
//	for(int i = 0; i < ShutterLEDController::BLINK_INTERVAL; i++){
//		c.run();
//		REQUIRE(MockPins::instance()->getPinState(greenpin) == HIGH);
//		REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//	}
//	c.run();
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//
//	//green pin HIGH after another BLINK_INTERVAL iterations
//	for(int i = 0; i < ShutterLEDController::BLINK_INTERVAL; i++){
//		c.run();
//		REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//		REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//	}
//	c.run();
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == HIGH);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//}
//
//TEST_CASE("Controllet Should go in to OPEN state"){
//	MockPins::instance()->clear();
//
//	//given a new ShutterLEDController
//	ShutterLEDController c(greenpin,redpin);
//
//	//on OPEN_BUTTON and CONNECT and SHUTTER_OPEN
//	c.onEvent(SHUTTER_OPEN_BUTTON_PRESS);
//	c.onEvent(PROJECTOR_CONNECTED);
//	c.onEvent(SHUTTER_OPENED);
//	c.run();
//
//	//should be in OPEN state
//	REQUIRE(c.getState() == ShutterLEDController::OPEN);
//	//green pin should be HIGH and red pin should be LOW
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == HIGH);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//}
//
//TEST_CASE("Controller should go in to CLOSING state"){
//	MockPins::instance()->clear();
//
//	//given a new ShutterLEDController
//	ShutterLEDController c(greenpin,redpin);
//
//	//on OPEN_BUTTON and CONNECT
//	c.onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
//	c.onEvent(PROJECTOR_CONNECTED);
//	c.onEvent(SHUTTER_OPENED);
//	c.run();
//
//	//should be in CLOSING state
//	REQUIRE(c.getState() == ShutterLEDController::CLOSING);
//
//	//green pin should be LOW and red pin should be HIGH
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == HIGH);
//
//	//red pin should be LOW after BLINK_INTERVAL iterations
//	for(int i = 0; i < ShutterLEDController::BLINK_INTERVAL; i++){
//		c.run();
//		REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//		REQUIRE(MockPins::instance()->getPinState(redpin) == HIGH);
//	}
//	c.run();
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//
//	//red pin HIGH after another BLINK_INTERVAL iterations
//	for(int i = 0; i < ShutterLEDController::BLINK_INTERVAL; i++){
//		c.run();
//		REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//		REQUIRE(MockPins::instance()->getPinState(redpin) == LOW);
//	}
//	c.run();
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == HIGH);
//}
//
//TEST_CASE("Controllet Should go in to CLOSED state"){
//	MockPins::instance()->clear();
//
//	//given a new ShutterLEDController
//	ShutterLEDController c(greenpin,redpin);
//
//	//on CLOSE_BUTTON and CONNECT and SHUTTER_OPEN and SHUTTER_CLOSE
//	c.onEvent(SHUTTER_CLOSE_BUTTON_PRESS);
//	c.onEvent(PROJECTOR_CONNECTED);
//	c.onEvent(SHUTTER_OPENED);
//	c.onEvent(SHUTTER_CLOSED);
//	c.run();
//
//	//should be in CLOSED state
//	REQUIRE(c.getState() == ShutterLEDController::CLOSED);
//	//green pin should be LOW and red pin should be HIGH
//	REQUIRE(MockPins::instance()->getPinState(greenpin) == LOW);
//	REQUIRE(MockPins::instance()->getPinState(redpin) == HIGH);
//}
