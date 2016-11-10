/*
 * Ethernet2.cpp
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#include <Ethernet2.h>
#include <iostream>

using namespace std;

EthernetClient::EthernetClient() : stopFlag(false), connectFlag(true), connectedFlag(true), pos(0) {

}

void EthernetClient::stop(){
	stopFlag = true;
}

void EthernetClient::print(const char* buffer) {
	printbuf = buffer;
//	cout << "PRINT" << endl;
//	cout << printbuf << endl;
}

char EthernetClient::read() {
	if(pos < readbuf.size()){
		return readbuf.at(pos++);
	} else {
		pos++;
		return -1;
	}
}

bool EthernetClient::connect(IPAddress ip, uint16_t p) {
	return connectFlag;
}

bool EthernetClient::available() {
	return pos < readbuf.length();
}

bool EthernetClient::connected() {
	return connectedFlag;
}
EthernetClient::~EthernetClient() {
}

void MockEthernetClient::setReadBuffer(const char* buffer) {
	readbuf = buffer;
	pos = 0;
}

std::string MockEthernetClient::getPrintBuffer() {
	string ret = printbuf;
	printbuf = "";
	return ret;
}

bool MockEthernetClient::getStopFlag() {
	bool s = stopFlag;
	stopFlag = false;
	return s;
}
void MockEthernetClient::setStopFlag(bool csf) {
	EthernetClient::stopFlag = csf;
}
void MockEthernetClient::setConnectFlag(bool cf) {
	EthernetClient::connectFlag = cf;
}

void MockEthernetClient::setConnectedFlag(bool cf) {
	EthernetClient::connectedFlag = cf;
}
