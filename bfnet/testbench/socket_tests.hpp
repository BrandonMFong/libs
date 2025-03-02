/**
 * author: Brando
 * date: 10/8/24
 */

#ifndef SOCKET_TESTS_HPP
#define SOCKET_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS
#define LOCALHOST "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

#include <bflibcpp/bflibcpp.hpp>
#include <socket.hpp>
#include <envelope.hpp>
#include <connection.hpp>
#include "bfnet_tests.hpp"
#include <unistd.h>

using namespace BF;
using namespace BF::Net;

void TestSocketPacketReceive(Envelope * envelope) { }

void TestSocketNewConnection(Connection * sc) { }

BFTEST_UNIT_FUNC(test_socketinitclient, 2<<10, {
	Socket * skt = Socket::create(SOCKET_MODE_CLIENT, LOCALHOST, PORT, &result);

	if (!skt) {
		result = 1;
	} else if (skt->mode() != SOCKET_MODE_CLIENT) {
		result = 2;
	} else if (skt->isReady()) {
		result = 3;
	}

	if (!result) {
		skt->setInStreamCallback(TestSocketPacketReceive);
		skt->setNewConnectionCallback(TestSocketNewConnection);
		skt->setBufferSize(BUFFER_SIZE);
		if (!skt->isReady()) {
			result = 4;
		}
	}

	BFRelease(skt);
})

BFTEST_UNIT_FUNC(test_socketinitserver, 2<<10, {
	Socket * skt = Socket::create(SOCKET_MODE_SERVER, LOCALHOST, PORT, &result);

	if (!skt) {
		result = 1;
	} else if (skt->mode() != SOCKET_MODE_SERVER) {
		result = 2;
	} else if (skt->port() != PORT) {
		result = 2;
	} else if (strcmp(skt->ipaddr(), LOCALHOST)) {
		result = 2;
	} else if (skt->isReady()) {
		result = 3;
	}

	if (!result) {
		skt->setInStreamCallback(TestSocketPacketReceive);
		skt->setNewConnectionCallback(TestSocketNewConnection);
		skt->setBufferSize(BUFFER_SIZE);
		if (!skt->isReady()) {
			result = 4;
		}
	}

	BFRelease(skt);
})

Atomic<Connection *> serverConn = NULL;
Atomic<Connection *> clientConn = NULL;
Data serverIn;
Atomic<bool> serverInReceived = false;
Data clientIn;
Atomic<bool> clientInReceived = false;

void test_sendingandreceiving_receive(
	Envelope * envelope,
	Data & in,
	Atomic<Connection *> & conn,
	Atomic<bool> & received
) {
	uuid_t u0, u1;
	envelope->connection()->getuuid(u0);
	conn.get()->getuuid(u1);
	if ((conn.get() != NULL) && !uuid_compare(u0, u1)) {
		in.alloc(envelope->data()->size(), (unsigned char *) envelope->data()->buffer());
		received = true;
	}
}

void test_sendingandreceiving_server_receive(Envelope * envelope) {
	test_sendingandreceiving_receive(envelope, serverIn, serverConn, serverInReceived);
}

void test_sendingandreceiving_server_new(Connection * sc) {
	serverConn = sc;
}

void test_sendingandreceiving_client_receive(Envelope * envelope) {
	test_sendingandreceiving_receive(envelope, clientIn, clientConn, clientInReceived);
}

void test_sendingandreceiving_client_new(Connection * sc) {
	clientConn = sc;
}

int test_sendingandreceiving_progress(const unsigned char * buf, size_t size) {
	return BUFFER_SIZE == size ? 1 : 0;
	//return -1;
}

BFTEST_UNIT_FUNC(test_sendingandreceiving, 1, {
	Socket * s = Socket::create(SOCKET_MODE_SERVER, LOCALHOST, PORT, &result);
	Socket * c = Socket::create(SOCKET_MODE_CLIENT, LOCALHOST, PORT, &result);

	if (!s || !c) {
		result = 1;
	} else {
		s->setInStreamCallback(test_sendingandreceiving_server_receive);
		s->setNewConnectionCallback(test_sendingandreceiving_server_new);
		s->setIncomingDataProgress(test_sendingandreceiving_progress);
		s->setBufferSize(BUFFER_SIZE);

		c->setInStreamCallback(test_sendingandreceiving_client_receive);
		c->setNewConnectionCallback(test_sendingandreceiving_client_new);
		c->setIncomingDataProgress(test_sendingandreceiving_progress);
		c->setBufferSize(BUFFER_SIZE);

		if (!s->isReady() || !c->isReady()) {
			result = 2;
		}
	}

	// start the connection
	
	if (!result) {
		result = s->start();
	}

	while (!result && !s->isRunning() && (serverConn.get() == NULL)) { usleep(50); }

	if (!result) {
		result = c->start();
	}
	
	while (!result && !c->isRunning() && (clientConn.get() == NULL)) { usleep(50); }

	int max = 2 << 12;
	while (!result && max--) {
		Data data(2 << 9); // test data
		srand(time(0));
		for (size_t i = 0; i < data.size(); i++) {
			unsigned char * buf = (unsigned char *) data.buffer();
			buf[i] = rand() % 256;
		}

		// send test data

		// client -> server
		if (!result) {
			serverInReceived = false; // reset
			serverIn.clear();

			if (!clientConn.get()->isactive()) {
				result = 1;
			} else {
				result = clientConn.get()->queueData(data.buffer(), data.size());
			}
		}
		
		while (!result && !serverInReceived && clientConn.get()->isactive() && serverConn.get()->isactive())
		{ usleep(50); }

		if (!result) {
			if (data != serverIn) {
				printf("\n%s != \n%s", data.hex().cString(), serverIn.hex().cString());
				printf("\n%ld != \n%ld", data.size(), serverIn.size());
				result = 3;
			}
		}

		// server -> client
		if (!result) {
			clientInReceived = false; // reset
			clientIn.clear();
			if (!serverConn.get()->isactive()) {
				result = 1;
			} else {
				result = serverConn.get()->queueData(data.buffer(), data.size());
			}
		}

		while (!result && !clientInReceived && clientConn.get()->isactive() && serverConn.get()->isactive())
		{ usleep(50); }

		if (!result) {
			if (data != clientIn) {
				result = 4;
			}
		}
	}

	if (!result) {
		result = c->stop();
	}

	if (!result) {
		result = s->stop();
	}

	BFRelease(s);
	BFRelease(c);
})

int test_sendingandreceiving_progress_forced_failure(const unsigned char * buf, size_t size) {
	return -1;
}

BFTEST_UNIT_FUNC(test_forcedFailureFromReceiver, 1, {
	Socket * s = Socket::create(SOCKET_MODE_SERVER, LOCALHOST, PORT, &result);
	Socket * c = Socket::create(SOCKET_MODE_CLIENT, LOCALHOST, PORT, &result);

	if (!s || !c) {
		result = 1;
	} else {
		s->setInStreamCallback(test_sendingandreceiving_server_receive);
		s->setNewConnectionCallback(test_sendingandreceiving_server_new);
		s->setIncomingDataProgress(test_sendingandreceiving_progress_forced_failure);
		s->setBufferSize(BUFFER_SIZE + 1);

		c->setInStreamCallback(test_sendingandreceiving_client_receive);
		c->setNewConnectionCallback(test_sendingandreceiving_client_new);
		c->setIncomingDataProgress(test_sendingandreceiving_progress_forced_failure);
		c->setBufferSize(BUFFER_SIZE + 1);

		if (!s->isReady() || !c->isReady()) {
			result = 2;
		}
	}

	// start the connection
	
	if (!result) {
		result = s->start();
	}

	while (!result && !s->isRunning() && (serverConn.get() == NULL)) { usleep(50); }

	if (!result) {
		result = c->start();
	}
	
	while (!result && !c->isRunning() && (clientConn.get() == NULL)) { usleep(50); }

	Data data(2 << 9); // test data
	srand(time(0));
	for (size_t i = 0; i < data.size(); i++) {
		unsigned char * buf = (unsigned char *) data.buffer();
		buf[i] = rand() % 256;
	}

	// send test data

	// client -> server
	if (!result) {
		serverInReceived = false; // reset
		serverIn.clear();

		if (!clientConn.get()->isactive()) {
			result = 1;
		} else {
			result = clientConn.get()->queueData(data.buffer(), data.size());
		}
	}
	
	while (!result && !serverInReceived && clientConn.get()->isactive() && serverConn.get()->isactive())
	{ usleep(50); }

	if (!result) {
		if (data != serverIn) {
			printf("\n%s != \n%s", data.hex().cString(), serverIn.hex().cString());
			printf("\n%ld != \n%ld", data.size(), serverIn.size());
			result = 3;
		}
	}

	// server -> client
	if (!result) {
		clientInReceived = false; // reset
		clientIn.clear();
		if (!serverConn.get()->isactive()) {
			result = 1;
		} else {
			result = serverConn.get()->queueData(data.buffer(), data.size());
		}
	}

	while (!result && !clientInReceived && clientConn.get()->isactive() && serverConn.get()->isactive())
	{ usleep(50); }

	if (!result) {
		if (data != clientIn) {
			result = 4;
		}
	}

	if (!result) {
		result = c->stop();
	}

	if (!result) {
		result = s->stop();
	}

	BFRelease(s);
	BFRelease(c);

})

BFTEST_COVERAGE_FUNC(socket_tests, {
	BFTEST_LAUNCH(test_socketinitclient);
	BFTEST_LAUNCH(test_socketinitserver);
	BFTEST_LAUNCH(test_sendingandreceiving);
	BFTEST_LAUNCH(test_forcedFailureFromReceiver);

})

#endif // SOCKET_TESTS_HPP

