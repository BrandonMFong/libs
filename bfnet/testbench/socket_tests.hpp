/**
 * author: Brando
 * date: 10/8/24
 */

#ifndef SOCKET_TESTS_HPP
#define SOCKET_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS
#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

#include <bflibcpp/bflibcpp.hpp>
#include <socket.hpp>
#include <envelope.hpp>
#include "bfnet_tests.hpp"
#include <unistd.h>

extern "C" {
//#include <bflibc/bflibc.h>
}

using namespace BF::Net;

void TestSocketPacketReceive(SocketEnvelope * envelope) { }

void TestSocketNewConnection(SocketConnection * sc) { }

int test_socketinitclient() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 10;

	while (!result && max--) {
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

		BFDelete(skt);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_socketinitserver() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 10;

	while (!result && max--) {
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

		BFDelete(skt);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void test_sendingandreceiving_server_receive(SocketEnvelope * envelope) { }
void test_sendingandreceiving_server_new(SocketConnection * sc) { }
void test_sendingandreceiving_client_receive(SocketEnvelope * envelope) { }
void test_sendingandreceiving_client_new(SocketConnection * sc) { }

int test_sendingandreceiving() {
	UNIT_TEST_START;
	int result = 0;
	int max = 1;

	while (!result && max--) {
		Socket * s = Socket::create(SOCKET_MODE_SERVER, LOCALHOST, PORT, &result);
		Socket * c = Socket::create(SOCKET_MODE_CLIENT, LOCALHOST, PORT, &result);

		if (!s || !c) {
			result = 1;
		} else {
			s->setInStreamCallback(test_sendingandreceiving_server_receive);
			s->setNewConnectionCallback(test_sendingandreceiving_server_new);
			s->setBufferSize(BUFFER_SIZE);

			c->setInStreamCallback(test_sendingandreceiving_client_receive);
			c->setNewConnectionCallback(test_sendingandreceiving_client_new);
			c->setBufferSize(BUFFER_SIZE);

			if (!s->isReady() || !c->isReady()) {
				result = 2;
			}
		}

		if (!result) {
			result = s->start();
		}

		while (!result && !s->isRunning()) { usleep(50); }

		if (!result) {
			result = c->start();
		}
		
		while (!result && !c->isRunning()) { usleep(50); }

		if (!result) {
			result = c->stop();
		}

		if (!result) {
			result = s->stop();
		}

		BFDelete(s);
		BFDelete(c);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void socket_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	//LAUNCH_TEST(test_socketinitclient, p, f);
	//LAUNCH_TEST(test_socketinitserver, p, f);
	LAUNCH_TEST(test_sendingandreceiving, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // SOCKET_TESTS_HPP

