/*
 * Arduino.cpp
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#include "Arduino.h"
#include <ctime>
#include <iostream>

MockMillis * MockMillis::instance(){
	static MockMillis instance;
	return &instance;
}

void MockMillis::setNext(unsigned long n){
	next = n;
}

unsigned long MockMillis::get(){
	return next++;
}

MockMillis::MockMillis(): next(50000){}


unsigned long millis(){
	return MockMillis::instance()->get();
}

void MockPins::digitalWrite(int pin, int state) {

//	std::cout << "WRITE PIN " << pin << " VALUE " << state << std::endl;
	auto i = pinState.find(pin);
	if(i == pinState.end()){
		pinState.insert(std::pair<int,int>(pin, state));
	} else {
		i->second = state;
	}
}

int MockPins::getPinState(int pin) {
	auto i = pinState.find(pin);
	if(i == pinState.end()){
		return -1;
	} else {
		return i->second;
	}
}

MockPins* MockPins::instance() {
	static MockPins instance;
	return &instance;
}

void digitalWrite(int pin, int state) {
	MockPins::instance()->digitalWrite(pin, state);
}

void MockPins::clear() {
	pinState.clear();
}
