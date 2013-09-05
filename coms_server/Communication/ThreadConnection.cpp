/*
 * ThreadConnection.cpp
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 */

#include "ThreadConnection.h"
#include "LinkHandler.h"
#include "CommunicationDevice.h"

#include "../Util/Log.h"


namespace slh {

ThreadConnection::ThreadConnection() {
}

ThreadConnection::~ThreadConnection() {
}

void ThreadConnection::setConnection(unsigned int iT, LinkHandler* iLinkHandler) {
	mT = iT;
	mLinkHandler = iLinkHandler;
}

int ThreadConnectionSend::sendall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return (n == -1) ? -1 : 0; // return -1 on failure, 0 on success
}

void ThreadConnectionSend::run() {
	pthread_mutex_lock(&mMutex);
	while (mvecEvent.size() > 0) {
		char tEvent = mvecEvent.front();
		mvecEvent.pop_front();

		int sendSize = 1;
		if (sendall(mT, &tEvent, &sendSize) == -1) {
			LOGD("TX Connection closed, stopping thread.");
			closesocket(mT);
			stopRun();
			mLinkHandler->linkDestroyNotify(mT);
		}
	}
	pthread_mutex_unlock(&mMutex);
	usleep(CONNECTION_LOOP_USLEEP);
}

void ThreadConnectionRecv::run() {

	int recvSize = 1;

	int rVal = recv(mT, mRecvBuffer, recvSize, 0);

	while (rVal > 0) {
		pthread_mutex_lock(&mMutex);
		mvecEvent.push_back(mRecvBuffer[0]);
		pthread_mutex_unlock(&mMutex);
		rVal = recv(mT, mRecvBuffer, 1, 0);
	}


	if (rVal <= 0) {
		// 0 means graceful EOF
		LOGD("RX Connection closed, stopping thread...");
		fflush(stdout);
		closesocket(mT);
		stopRun();
		mLinkHandler->linkDestroyNotify(mT);
	}

}

}
