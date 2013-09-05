/*
 * Thread.h
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

namespace slh {

class Thread {
protected:
    pthread_t mThread;
    pthread_mutex_t mMutex;
    unsigned int mId;
    bool mThreadStopped;

	void stopRun();
	void loop();
	virtual void run() = 0;

	// static function to map overloaded C++ method into a C function pointer (needed for pthread)
	static void* threadStart(void *obj)
	{
		reinterpret_cast<Thread*>(obj)->loop();
		return NULL;
	}

public:
	Thread();
	virtual ~Thread();

	void go();
	void stop();
	bool stopped();

};

}

#endif /* THREAD_H_ */
