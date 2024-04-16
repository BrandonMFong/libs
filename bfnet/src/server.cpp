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

Server::Server() : Socket() {
	this->_mainSocket = 0;
	this->_pollt = NULL;
}

Server::~Server() {
	BFThreadAsyncDestroy(this->_pollt);
}

const char Server::mode() const {
	return SOCKET_MODE_SERVER;
}

void Server::init(void * in) {
	Server * s = (Server *) in;

	BFRetain(s);

	// create server socket similar to what was done in
    // client program
    s->_mainSocket = socket(AF_INET, SOCK_STREAM, 0);

    // define server address
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(s->port());
    servAddr.sin_addr.s_addr = inet_addr(s->ipaddr());

    // bind socket to the specified IP and port
    bind(s->_mainSocket, (struct sockaddr *) &servAddr, sizeof(servAddr));

    // listen for connections
    listen(s->_mainSocket, 5);

	// launch thread that will constantly accept multiple connections
	s->_pollt = BFThreadAsync(Server::pollthread, s);

	BFRelease(s);
}

void Server::pollthread(void * in) {
	Server * s = (Server *) in;
	BFRetain(s);

	while (!BFThreadAsyncIsCanceled(s->_pollt)) {
		int csock = accept(s->_mainSocket, NULL, NULL); // this blocks

		int err = 0;
		SocketConnection * sc = new SocketConnection(csock, s);
		if (!sc) {
			err = 1;
		}

		if (!err) {
			if (s->_cbnewconn)
				err = s->_cbnewconn(sc);

			if (err) {
			}
		}

		// if there are no errors with connection then
		// we will add it to our connections list
		if (!err) {
			s->_connections.get().add(sc);
			s->startInStreamForConnection(sc);
		}
	}

	BFRelease(s);
}

int Server::_start() {
	BFThreadAsyncID tid = BFThreadAsync(Server::init, this);
	BFThreadAsyncDestroy(tid);
	
	return 0;
}

int Server::_stop() {
	shutdown(this->_mainSocket, SHUT_RDWR);
	close(this->_mainSocket);

	// thread break down
	BFThreadAsyncCancel(this->_pollt);
	BFThreadAsyncWait(this->_pollt);

	return 0;
}

