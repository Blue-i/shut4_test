/*
 * MockDevice.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#include <MockDevice.h>
#include <cstring>
#include <iostream>

MockDevice::MockDevice() : resetFlag(false){
	// TODO Auto-generated constructor stub

}

MockDevice::~MockDevice() {
	// TODO Auto-generated destructor stub
}

bool MockDevice::addComponent(Component<Device>* component) {
	return true;
}

bool MockDevice::removeComponent(Component<Device>* component) {
	return true;
}

size_t MockDevice::readFor(char* buffer, size_t size, uint16_t timeout) {
	strncpy(buffer, readbuf.c_str(), size-1);
	buffer[size-1] = '\0';
	size_t s = readbuf.length();
	readbuf = "";
	return s;
}

void MockDevice::write(const char buffer[]) {
	writebuf = buffer;
}

void MockDevice::reset() {
	resetFlag = true;
}

void MockDevice::setReadbuf(const char* str) {
	readbuf = str;
}

std::string MockDevice::getWritebuf() {
	std::string s = writebuf;
	writebuf = "";
	return s;
}

bool MockDevice::isReset() {
	bool b = resetFlag;
	resetFlag = false;
	return b;
}
