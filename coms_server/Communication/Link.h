/*
 * Link.h
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 *
 *  Composites Send and Receive Threads to represent a Duplex TCP/IP Link between the client and server
 *  go() accepts an observer for "callbacks".
 */

#ifndef LINK_H_
#define LINK_H_

#include "ThreadConnection.h"
#include "LinkHandler.h"

namespace slh {

class Link {
	ThreadConnectionRecv mThreadRecv;
	ThreadConnectionSend mThreadSend;

public:
	Link();
	virtual ~Link();
	void go(unsigned int iT, LinkHandler* iLinkHandler);
	void stop();
	void pushEvent(char iEvent);
	char popEvent();

};

}

#endif /* LINK_H_ */
