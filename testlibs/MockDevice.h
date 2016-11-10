/*
 * MockDevice.h
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#ifndef MOCKDEVICE_H_
#define MOCKDEVICE_H_

#include <Device.h>
#include <string>

class MockDevice: public Device {
	bool resetFlag;
	std::string readbuf;
	std::string writebuf;

public:
	MockDevice();
	virtual ~MockDevice();

	virtual bool addComponent(Component<Device>*);
	virtual bool removeComponent(Component<Device>*);
	virtual size_t readFor(char * buffer, size_t size, uint16_t timeout);
	virtual void write(const char buffer[]);
	virtual void reset();

	void setReadbuf(const char *);
	std::string getWritebuf();
	bool isReset();

};

#endif /* MOCKDEVICE_H_ */
