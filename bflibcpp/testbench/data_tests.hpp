/**
 * author: Brando
 * date: 1/24/24
 */

#ifndef DATA_TESTS_HPP
#define DATA_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <data.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_datainit() {
	UNIT_TEST_START;
	int result = 0;

	Data buf0;
	if (buf0.size() != 0) {
		result = 1;
	}

	if (!result) {
		unsigned char bytes[10];
		Data buf1(sizeof(bytes), bytes);

		if (buf1.size() != sizeof(bytes)) {
			result = 2;
		}
	}

	if (!result) {
		Data buf2(32);
		if (buf2.size() != 32) {
			result = 3;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_clearData() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 4;
	const size_t maxbufsize = 2 << 16;

	while (max-- && !result) {
		srand(time(0));
		const size_t s = rand() % maxbufsize;
		Data buf(s);
		if (buf.size() != s) {
			result = 1;
		}

		void * bytes = malloc(buf.size());
		if (!result) {
			memcpy(buf.buffer(), bytes, buf.size());
			if (memcmp(buf.buffer(), bytes, buf.size())) {
				result = 2;
			}
		}

		if (!result) {
			buf.clear();
			memset(bytes, 0, buf.size());
			if (memcmp(buf.buffer(), bytes, buf.size())) {
				result = 3;
			}
		}
		BFFree(bytes);
	}

	UNIT_TEST_END(!result, result);
	return result;
}


void data_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_datainit, p, f);
	LAUNCH_TEST(test_clearData, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // DATA_TESTS_HPP

