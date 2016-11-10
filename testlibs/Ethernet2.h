/*
 * Ethernet2.h
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#ifndef ETHERNET2_H_
#define ETHERNET2_H_

#include <string>

class IPAddress{

};

class EthernetClient {

protected:
	bool stopFlag;
	bool connectFlag;
	bool connectedFlag;
	std::string printbuf;
	std::string readbuf;
	std::size_t pos;

public:
	EthernetClient();
	void print(const char * buffer);
	bool available();
	bool connected();
	bool connect(IPAddress, uint16_t);
	void stop();
	void flush(){};
	char read();
	virtual ~EthernetClient();
};

class MockEthernetClient : public EthernetClient {
public:
	void setReadBuffer(char const *);
	std::string getPrintBuffer();
	bool getStopFlag();
	void setStopFlag(bool);
	void setConnectFlag(bool);
	void setConnectedFlag(bool);
};
#endif /* ETHERNET2_H_ */
