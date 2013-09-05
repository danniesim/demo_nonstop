/*
 * ThreadConnection.h
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 *
 * Represents a TCP/IP Connection running on a Thread.
 * Note separate threads for Sending (ThreadConnectionSend) and Receiving (ThreadConnectionSend)
 */


#ifndef THREADCONNECTION_H_
#define THREADCONNECTION_H_

#include "../Os/Thread.h"
#include <list>

namespace slh {

class LinkHandler;

const int CONNECTION_LOOP_USLEEP = 10000;
const int CONNECTION_SELECT_USLEEP = 10000;
const int CONNECTION_DEFAULT_PORTNUM = 50000;
const int CONNECTION_MAX_HOSTNAME = 255;
const int CONNECTION_MAX_BUFFER = 256;

class ThreadServer;

class ThreadConnection :public Thread {
friend class Link;
protected:
    unsigned int mT;
    LinkHandler* mLinkHandler;
    std::list<char> mvecEvent;

public:
	ThreadConnection();
	virtual ~ThreadConnection();

	void setConnection(unsigned int iT, LinkHandler* iLinkHandler);
};

class ThreadConnectionSend :public ThreadConnection {
	int sendall(int s, char *buf, int *len);
	void run();
};

class ThreadConnectionRecv :public ThreadConnection {
    char mRecvBuffer[CONNECTION_MAX_BUFFER];
	void run();
};

}

#endif /* THREADCONNECTION_H_ */
