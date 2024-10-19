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
	} else if (buf0.buffer() == NULL) {
		result = 4;
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

	if (!result) {
		unsigned char bytes[10];
		Data buf3(sizeof(bytes), bytes);
		Data buf4 = buf3;

		if (buf4.size() != sizeof(bytes)) {
			result = 4;
		} else if (buf4.size() != buf3.size()) {
			result = 5;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_clearData() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 12;
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

int test_decreasingSize() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 12;

	while (!result && max--) {
		srand(time(0));
		size_t size = rand() % (2 << 16);
		char * bytes = (char *) malloc(size);
		Data buf(size, (unsigned char *) bytes);
		size_t newsize = size - (rand() % (size / 2));
		buf.resize(newsize);
		if (buf.size() != newsize) {
			result = 1;
		}

		if (!result) {
			for (int i = 0; i < (int) newsize; i++) {
				if (((char *) buf.buffer())[i] != bytes[i]) {
					result = 2;
					break;
				}
			}
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_increasingSize() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 12;

	while (!result && max--) {
		srand(time(0));
		size_t size = rand() % (2 << 16);
		char * bytes = (char *) malloc(size);
		Data buf(size, (unsigned char *) bytes);
		size_t newsize = size + (rand() % (size / 2));
		buf.resize(newsize);
		if (buf.size() != newsize) {
			result = 1;
		}

		// compare only old size
		if (!result) {
			for (int i = 0; i < (int) size; i++) {
				if (((char *) buf.buffer())[i] != bytes[i]) {
					result = 2;
					break;
				}
			}
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_String2Data() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 10;

	while (!result && max--) {
		String str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
		Data buf = str;

		const char * tmp = (const char *) buf.buffer();
		if (strlen(tmp) != str.length()) {
			result = 1;
		} else if (str.compareString(tmp)) {
			result = 2;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_HexString() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 10;

	while (!result && max--) {
		srand(time(0));
		size_t size = rand() % 1024;
		void * buf = malloc(size);
		if (buf == NULL) {
			result = 1;
		}

		Data * data = NULL;
		if (!result) {
			data = new Data(size, (unsigned char *) buf);
			result = data == NULL ? 2 : result;
		}

		if (!result) {
			if (data->hex().length() != (size * 2)) {
				result = 3;
			}
		}

		BFFree(buf);
		BFDelete(data);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_dataCompare() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 15;

	while (!result && max--) {
		srand(time(0));
		int size = rand() % 2048;
		unsigned char * b0 = (unsigned char *) malloc(size);
		unsigned char * b1 = (unsigned char *) malloc(size);

		for (int i = 0; i < size; i++) {
			b0[i] = rand() % (2 << 7);
			b1[i] = rand() % (2 << 7);
		}

		if (b0 == NULL) {
			result = 1;
			break;
		} else if (b1 == NULL) {
			result = 2;
			break;
		} else if (memcmp(b0, b1, size) == 0) {
			result = 6;
			break;
		}

		Data d0(size, b0);
		Data d1(size, b0);
		Data d2(size, b1);

		if (d0 != d1) {
			result = 3;
			break;
		} else if (d0 == d2) {
			result = 4;
			break;
		} else if (d1 == d2) {
			result = 5;
			break;
		}

		BFFree(b0);
		BFFree(b1);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_emptyStringDataLength() {
	UNIT_TEST_START;
	int result = 0;
	
	String str = "";
	if (str.length() != 0) {
		result = 1;
	}

	if (!result) {
		Data d = str;
		if (d.size() != 1) {
			result = 2;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

Data test_dataByRefBuf(32);
void test_dataByRefCallback(Data & d) {
	d = test_dataByRefBuf;
}

int test_dataByRef() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 20;

	while (!result && max--) {
		unsigned char * tmp = (unsigned char *) test_dataByRefBuf.buffer();
		srand(time(0));
		for (size_t i = 0; i < test_dataByRefBuf.size(); i++) {
			tmp[i] = rand() % (2 << 7);
		}

		Data d;
		test_dataByRefCallback(d);

		if (d != test_dataByRefBuf) {
			result = max;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void data_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_datainit, p, f);
	LAUNCH_TEST(test_clearData, p, f);
	LAUNCH_TEST(test_decreasingSize, p, f);
	LAUNCH_TEST(test_increasingSize, p, f);
	LAUNCH_TEST(test_String2Data, p, f);
	LAUNCH_TEST(test_HexString, p, f);
	LAUNCH_TEST(test_dataCompare, p, f);
	LAUNCH_TEST(test_emptyStringDataLength, p, f);
	LAUNCH_TEST(test_dataByRef, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // DATA_TESTS_HPP

