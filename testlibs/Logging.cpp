/*
 * Logging.cpp
 *
 *  Created on: 31 Oct 2016
 *      Author: rag
 */

#include <Logging.h>
#include <iostream>

using namespace std;
Logging::Logging() : log(false) {
	// TODO Auto-generated constructor stub

}

Logging::~Logging() {
	// TODO Auto-generated destructor stub
}

void Logging::Debug(char const * msg, ...) {
	if(log){
		cout << msg << endl;
	}
}

Logging Log = Logging();

void Logging::on() {
	log = true;
}

void Logging::off() {
	log = false;
}
