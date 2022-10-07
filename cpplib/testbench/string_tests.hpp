/**
 * author: Brando
 * date: 10/4/22
 */

#ifndef STRING_TESTS_HPP
#define STRING_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "../string.hpp"

extern "C" {
#include <clib.h>
}

int test_StringInit() {
	int result = 0;

	char str[100] = "hello world";

	String s0 = str;
	String s1 = "hello world";
	String s2("hello world");
	String s3(str);

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_comparingString() {
	int result = 0;

	char tmp[20] = "hello world";

	String str0 = tmp;
	String str1;

	if (strcmp(str0, tmp)) {
		std::cout << str0 << " != " << tmp << std::endl;
		result = 1;
	}

	if (!result) {
		str1 = "hello world";

		result = (str0 == str1) ? 0 : 2;

		if (result) {
			std::cout << str0 << " != " << str1 << std::endl;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_StringLength() {
	int result = 0;

	const char * tmp = "Hello, my name is brando";

	String str = tmp;

	if (str.length() != strlen(tmp)) {
		result = 1;
		printf("%d != %d\n", (int) str.length(), (int) strlen(tmp));
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

void string_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_StringInit()) p++;
	else f++;

	if (!test_comparingString()) p++;
	else f++;

	if (!test_StringLength()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STRING_TESTS_HPP

