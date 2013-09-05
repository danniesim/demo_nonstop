/*
 * LinkHandler.h
 *
 *  Created on: Oct 25, 2011
 *      Author: prophesci
 */

#ifndef LINKHANDLER_H_
#define LINKHANDLER_H_

#include "ThreadConnection.h"

namespace slh {

class LinkHandler {
	friend class ThreadConnectionSend;
	friend class ThreadConnectionRecv;
protected:
	virtual void linkCreateNotify(unsigned int iT) = 0;
	virtual void linkDestroyNotify(unsigned int iT) = 0;
};

}

#endif /* LINKHANDLER_H_ */
