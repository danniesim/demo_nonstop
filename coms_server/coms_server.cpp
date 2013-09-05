/*
 * coms_server.cpp
 *
 *  Created on: Oct 23, 2011
 *      Author: prophesci
 *
 * Multi-client TCP/IP communications server
 * Current test configuration implements a Telnet-able echo server.
 * Clients type '~' to close connection.
 */

#include <string>
#include "Util/Log.h"

#include "Communication/CommunicationDevice.h"
#include "Os/Thread.h"
#include "Communication/ThreadServer.h"

using namespace std;
using namespace slh;

int main() {
	bool bShutdown = false;
	ThreadServer threadServer;
	MapLink mapLink;

	CommunicationDevice::create();
	threadServer.create("192.168.0.100", CONNECTION_DEFAULT_PORTNUM);

	if (threadServer.isListening()) {
		threadServer.go();
		do {
			// Handle new links
			unsigned int insertId = threadServer.linkCreatePop();
			if (insertId > 0) {
				mapLink[insertId] = threadServer.link(insertId);
				string welcomeMsg = "*** Welcome to PATH OF HEROES anonymous chat server! ***\n";
				for (unsigned int i = 0; i < welcomeMsg.size(); i++) {
					mapLink[insertId]->pushEvent(welcomeMsg[i]);
				}
			}

			// Handle closed links
			unsigned int removeId = threadServer.linkDestroyPop();
			if (removeId > 0) {
				mapLink.erase(removeId);
				threadServer.linkDestroy(removeId);
			}

			// Broadcast chat code
			for (MapLink::iterator it = mapLink.begin(); it != mapLink.end(); it++) {
				char tEvent = it->second->popEvent();
				while (tEvent > 0) {
					if (tEvent == '~') {
						bShutdown = true;
					}

					for (MapLink::iterator it2 = mapLink.begin(); it2 != mapLink.end(); it2++) {
						if (it != it2) {
							it2->second->pushEvent(tEvent);
						}
					}
					tEvent = it->second->popEvent();
				}
			}

			usleep(CONNECTION_LOOP_USLEEP);
		} while(bShutdown != true);
	} else {
		LOGE("Cannot establish listening socket!");
	}

	threadServer.destroy();
	CommunicationDevice::destroy();

	return 0;
}

