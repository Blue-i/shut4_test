/*
 * Arduino.h
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#ifndef TESTLIBS_ARDUINO_H_
#define TESTLIBS_ARDUINO_H_
#include <cstddef>
#include <map>

typedef unsigned long size_t;

class MockMillis {
	MockMillis();
	unsigned long next;
public:
	void setNext(unsigned long next);
	unsigned long get();
	static MockMillis * instance();
};

#define LOW 0
#define HIGH 1

class MockPins {
	std::map<int, int> pinState;
public:
	void digitalWrite(int, int);
	int getPinState(int);
	void clear();
	static MockPins * instance();
};

void digitalWrite(int, int);
unsigned long millis();

#endif /* TESTLIBS_ARDUINO_H_ */
