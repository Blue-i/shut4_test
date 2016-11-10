/*
 * MockComponent.h
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#ifndef MOCKCOMPONENT_H_
#define MOCKCOMPONENT_H_

#include "Component.h"
#include "Device.h"

class MockComponent : public Component<Device> {
	Device * device;
	bool resetFlag;
	bool runFlag;
public:
	MockComponent();
	virtual ~MockComponent();

	virtual void bind(Device*, char*, size_t);
	virtual bool isBound();
	virtual void run();
	virtual void reset();

	bool isReset();
	bool isRun();
};

#endif /* MOCKCOMPONENT_H_ */
