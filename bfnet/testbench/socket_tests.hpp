/**
 * author: Brando
 * date: 10/8/24
 */

#ifndef SOCKET_TESTS_HPP
#define SOCKET_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS
#define LOCALHOST "127.0.0.1"
#define PORT 8080

#include <bflibcpp/bflibcpp.hpp>
#include <socket.hpp>
#include "bfnet_tests.hpp"

extern "C" {
//#include <bflibc/bflibc.h>
}

using namespace BF::Net;

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
		} else {
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

