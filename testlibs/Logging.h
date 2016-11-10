/*
 * Logging.h
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#ifndef LOGGING_H_
#define LOGGING_H_


#define CR "\r\n"

class Logging {
	bool log;
public:
	void Debug(char const * msg, ...);
	void on();
	void off();
	Logging();
	virtual ~Logging();
};

extern Logging Log;

#endif /* LOGGING_H_ */
