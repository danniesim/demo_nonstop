/*
 * Thread.cpp
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 */

#include "Thread.h"
#include "../Util/Log.h"
#include <assert.h>

namespace slh {

Thread::Thread() {
	mThreadStopped = true;
	pthread_mutex_init(&mMutex, NULL);
}

Thread::~Thread() {
}


void Thread::stopRun() {
	mThreadStopped = true;
}

void Thread::loop() {
	while (!mThreadStopped) {
		run();
	}
	LOGD("Thread loop ended!");
}

// Create the thread and start work
void Thread::go() {
	assert(mThreadStopped == true);
	mThreadStopped = false;

	pthread_create(&mThread, 0, &Thread::threadStart, this);
}

void Thread::stop() {
//		assert(mStopThread == false);
	mThreadStopped = true;
	pthread_join(mThread, 0);
}

bool Thread::stopped() {
	return mThreadStopped;
}


}
