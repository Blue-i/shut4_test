/*
 * os48.h
 *
 *  Created on: 1 Nov 2016
 *      Author: rag
 */

#ifndef OS48_H_
#define OS48_H_

namespace os48 {
#define ATOMIC_BLOCK(type)
#define OS48_ATOMIC_BLOCK             \
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)

#define OS48_NO_CS_BLOCK

class Mutex {
public:
	Mutex(){};
	virtual ~Mutex(){};

	void lock(){};
	void unlock(){};
};
}


#endif /* OS48_H_ */
