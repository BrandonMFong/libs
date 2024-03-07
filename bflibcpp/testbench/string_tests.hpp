/**
 * author: Brando
 * date: 10/4/22
 */

#ifndef STRING_TESTS_HPP
#define STRING_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <string.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

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

int test_StringCopy() {
	int result = 0;

	String str0 = "Hello world";
	String str1;

	result = str0.copy(str1);

	if (result) {
		printf("Error %d\n", result);
	} else if (str0 != str1) {
		printf("%s != %s\n", str0.cString(), str1.cString());
		result = 1;
	}

	if (!result) {
		str0 = "Hello world!";
		str1 = str0;

		if (str0 != str1) {
			result = 2;
			printf("%s != %s\n", (const char *) str0, (const char *) str1);
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

bool DoStringsMatch(String str, const char * expected) {
	return !strcmp(str.cString(), expected);
}

int test_PassingStringToFunction() {
	int result = 0;

	if (!DoStringsMatch("Hello", "Hello")) {
		result = 1;
	} else if (DoStringsMatch("Hello", "hello")) {
		result = 2;
	}

	if (!result) {
		String a = "World";

		// TODO: this crashes
		if (!DoStringsMatch(a, "World")) {
			result = 3;
		} else if (DoStringsMatch(a, "world")) {
			result = 4;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_removingCharacterFromString() {
	UNIT_TEST_START;
	int result = 0;
	
	const char * s = "hello world!";
	const char * e = "hello world";

	int max = 2 << 24;
	while (!result && max) {
		String str = s;

		str.remChar();

		if (str.compareString(e)) {
			printf("\n%s != %s\n", str.cString(), e);
			result = max;
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_addingCharacterToString() {
	UNIT_TEST_START;
	int result = 0;
	
	const char * s = "hello world";
	const char * e = "hello world!";

	int max = 2 << 24;
	while (!result && max) {
		String str = s;

		str.addChar('!');

		if (str.compareString(e)) {
			printf("\n%s != %s\n", str.cString(), e);
			result = max;
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_clearstring() {
	UNIT_TEST_START;
	int result = 0;
	
	int max = 2 << 10;
	String str;
	while (!result && max) {
		int size = (2 << 10);

		for (int i = 0; i < size; i++) {
			str.addChar('.');
		}

		if (str.length() == 0) {
			result = max;
		}

		if (!result) {
			str.clear();
			if (str.length() > 0) {
				printf("\nlen=%ld, '%s'\n", str.length(), str.cString());
				result = max;
			}
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_indexingstring() {
	UNIT_TEST_START;
	int result = 0;
	
	const char * s = "Honorificabilitudinitatibus";

	int max = 2 << 24;
	while (!result && max) {
		String str = s;

		for (size_t i = 0; i < str.length(); i++) {
			if (str[i] != s[i]) {
				result = max;
			}
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_removingcharatindex() {
	UNIT_TEST_START;
	int result = 0;
	
	char s[128];
	strncpy(s, "Honorificabilitudinitatibus", sizeof(s));

	int max = 2 << 12;
	while (!result && max) {
		String str = s;

		srand(time(0));
		size_t index = rand() % (str.length() - 1);

		str.remCharAtIndex(index);

		char * tmp = s;
		for (size_t i = 0; i < str.length(); i++) {
			if (i == index)
				tmp++;

			if (str[i] != tmp[0]) {
				printf("\n%s != %s\n", str.cString(), s);
				printf("\n%c != %c\n", str[i], tmp[0]);
				result = max;
				break;
			}
			tmp++;
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_addandremove() {
	UNIT_TEST_START;
	int result = 0;
	
	const char * s = "Honorificabilitudinitatibus";

	int max = 2 << 12;
	while (!result && max) {
		String str;

		for (int i = strlen(s) - 1; i >= 0; i--) {
			str.addCharAtIndex(s[i], 0);
		}

		if (str.compareString(s)) {
			result = max;
		}

		while (!result && str.length()) {
			str.remCharAtIndex(0);
		}

		if (!result) {
			if (str.length()) {
				result = max;
			}
		}

		max--;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_stringtoint() {
	UNIT_TEST_START;
	int result = 0;
	
	int max = 2 << 12;
	while (!result && max--) {
		char num[1024];
		snprintf(num, 1024, "%d", max);
		String str(num);
		int val = String::toi(str);
		if (val != max) {
			result = max;
		}
	}

	UNIT_TEST_END(!result, result);
	return result;

}

void string_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_StringInit, p, f);
	LAUNCH_TEST(test_comparingString, p, f);
	LAUNCH_TEST(test_StringLength, p, f);
	LAUNCH_TEST(test_StringCopy, p, f);
	LAUNCH_TEST(test_PassingStringToFunction, p, f);
	LAUNCH_TEST(test_addingCharacterToString, p, f);
	LAUNCH_TEST(test_clearstring, p, f);
	LAUNCH_TEST(test_removingCharacterFromString, p, f);
	LAUNCH_TEST(test_removingcharatindex, p, f);
	LAUNCH_TEST(test_indexingstring, p, f);
	LAUNCH_TEST(test_addandremove, p, f);
	LAUNCH_TEST(test_stringtoint, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STRING_TESTS_HPP

