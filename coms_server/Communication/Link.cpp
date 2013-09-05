/*
 * Link.cpp
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 */

#include "Link.h"

namespace slh {

Link::Link() {
}

Link::~Link() {
}

void Link::go(unsigned int iT, LinkHandler* iLinkHandler) {
	mThreadSend.setConnection(iT, iLinkHandler);
	mThreadSend.go();
	mThreadRecv.setConnection(iT, iLinkHandler);
	mThreadRecv.go();
}

void Link::stop() {
	mThreadSend.stop();
	mThreadRecv.stop();
}

void Link::pushEvent(char iEvent) {
	pthread_mutex_lock(&(mThreadSend.mMutex));
	mThreadSend.mvecEvent.push_back(iEvent);
	pthread_mutex_unlock(&(mThreadSend.mMutex));
}

char Link::popEvent() {
	char rVal = 0;

	pthread_mutex_lock(&(mThreadRecv.mMutex));
	if (mThreadRecv.mvecEvent.size() > 0) {
		rVal = mThreadRecv.mvecEvent.front();
		mThreadRecv.mvecEvent.pop_front();
	}
	pthread_mutex_unlock(&(mThreadRecv.mMutex));

	return rVal;
}

}
