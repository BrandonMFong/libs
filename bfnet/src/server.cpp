/**
 * author: brando
 * date: 1/24/24
 */

#include "server.hpp"
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

BF::Net::Server::Server() : Socket() {
	this->_mainSocket = 0;
	this->_pollt = NULL;
}

BF::Net::Server::~Server() {
	BFThreadAsyncDestroy(this->_pollt.get());
}

const char BF::Net::Server::mode() const {
	return SOCKET_MODE_SERVER;
}

void BF::Net::Server::init(void * in) {
	Server * s = (Server *) in;

	BFRetain(s);

	// create server socket similar to what was done in
    // client program
    s->_mainSocket = socket(AF_INET, SOCK_STREAM, 0);
	int err = 0;
	if (s->_mainSocket == -1) {
		BFNetLogDebug("%s - socket() returned %d", __FUNCTION__, errno);
		err = 1;
	}

    // define server address
    struct sockaddr_in servAddr;
	if (!err) {
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(s->port());
		servAddr.sin_addr.s_addr = inet_addr(s->ipaddr());

		// bind socket to the specified IP and port
		err = bind(s->_mainSocket.get(), (struct sockaddr *) &servAddr, sizeof(servAddr));
		if (err) {
			BFNetLogDebug("%s - bind() returned %d", __FUNCTION__, errno);
		}
	}

	if (!err) {
		// listen for connections
		err = listen(s->_mainSocket.get(), 5);
		if (err) {
			BFNetLogDebug("%s - listen() returned %d", __FUNCTION__, errno);
		}
	}

	if (!err) {
		// launch thread that will constantly accept multiple connections
		s->_pollt = BFThreadAsync(Server::pollthread, s);
	}

	BFRelease(s);
}

void BF::Net::Server::pollthread(void * in) {
	Server * s = (Server *) in;
	BFRetain(s);

	int err = 0;
	while (!err && !BFThreadAsyncIsCanceled(s->_pollt.get())) {
		int csock = accept(s->_mainSocket.get(), NULL, NULL); // this blocks
		if (csock == -1) {
			BFNetLogDebug("%s - accept() returned %d", __FUNCTION__, errno);
			err = 1;
		}

		SocketConnection * sc = NULL;
		if (!err) {
			sc = new SocketConnection(csock, s);
			if (!sc) {
				BFNetLogDebug("%s - could not create SocketConnection", __FUNCTION__);
				err = 1;
			}
		}

		// if there are no errors with connection then
		// we will add it to our connections list
		if (!err) {
			if (s->_cbnewconn)
				s->_cbnewconn(sc);

			s->_connections.get().add(sc);
			s->startInStreamForConnection(sc);
		}
	}

	BFRelease(s);
}

bool BF::Net::Server::isRunning() const {
	if (!BFThreadAsyncIDIsValid(this->_pollt.get()))
		return false;
	else if (!BFThreadAsyncIsRunning(this->_pollt.get()))
		return false;

	return true;
}

int BF::Net::Server::_start() {
	BFThreadAsyncID tid = BFThreadAsync(Server::init, this);
	BFThreadAsyncDestroy(tid);
	
	return 0;
}

int BF::Net::Server::_stop() {
	shutdown(this->_mainSocket.get(), SHUT_RDWR);
	close(this->_mainSocket.get());

	// thread break down
	BFThreadAsyncCancel(this->_pollt.get());
	BFThreadAsyncWait(this->_pollt.get());

	return 0;
}

