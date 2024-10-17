/**
 * author: brando
 * date: 1/24/24
 */

#include "client.hpp"
#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <unistd.h>
#include <bflibcpp/bflibcpp.hpp>
#include "connection.hpp"
#include <arpa/inet.h>
#include "internal/log.hpp"

using namespace BF::Net;

BF::Net::Client::Client() { }

BF::Net::Client::~Client() { }

const char BF::Net::Client::mode() const {
	return SOCKET_MODE_CLIENT;
}

void BF::Net::Client::init(void * in) {
	Client * c = (Client *) in;

	BFRetain(c);

	int sock = 0;	
	sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(c->port());
    servAddr.sin_addr.s_addr = inet_addr(c->ipaddr());

    int connectStatus = connect(
		sock,
		(struct sockaddr *) &servAddr,
		sizeof(servAddr)
	);

	int err = 0;
    if (connectStatus == -1) {
		err = errno;
	}

	SocketConnection * sc = NULL;
	if (!err) {
		sc = new SocketConnection(sock, c);
	}

	if (!err) {
		if (c->_cbnewconn)
			c->_cbnewconn(sc);

		c->_connections.get().add(sc);
		c->startInStreamForConnection(sc);
    }

	if (err) {
		BFNetLogDebug("%s - %d\n", __func__, err);
	}

	BFRelease(c);
}

bool BF::Net::Client::isRunning() const {
	if (this->_tidin.get().count() != 1) { // assuming client will only connct to 1 server
		//BFNetLogDebug("%s - count == %d\n", __func__, this->_tidin.get().count());
		return false;
	} else if (!BFThreadAsyncIDIsValid(this->inStreamThreadID())) {
		//BFNetLogDebug("%s - invalid thread id\n", __func__);
		return false;
    } else if (!BFThreadAsyncIsRunning(this->inStreamThreadID())) {
		//BFNetLogDebug("%s - thread not running\n", __func__);
		return false;
	}

	return true;
}

BFThreadAsyncID Client::inStreamThreadID() const {
	return this->_tidin.get().first()->object();
}

int BF::Net::Client::_start() {
	BFThreadAsyncID tid = BFThreadAsync(Client::init, this);
	BFThreadAsyncDestroy(tid);
	
	return 0;
}

int BF::Net::Client::_stop() {
	return 0;
}

