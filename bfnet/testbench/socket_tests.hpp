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

extern "C" {
//#include <bflibc/bflibc.h>
}

using namespace BF::Net;

void TestSocketPacketReceive(SocketEnvelope * envelope) {
	BFRetain(envelope);
	BFRelease(envelope);
}

int TestSocketNewConnection(SocketConnection * sc) {
	return 0;
}

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

void socket_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_socketinitclient, p, f);
	LAUNCH_TEST(test_socketinitserver, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // SOCKET_TESTS_HPP

