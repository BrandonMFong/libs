/**
 * author: Brando
 * date: 10/4/22
 */

#ifndef STRING_TESTS_HPP
#define STRING_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <string.hpp>
#include <data.hpp>
#include <release.hpp>

extern "C" {
#include <bflibc/bflibc.h>
#include <string.h>
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

int test_nullstring(void) {
	UNIT_TEST_START;
	int result = 0;

	String str0 = 0;
	String str1 = 0;

	if (str0.length()) result = 1;
	else if (str1.length()) result = 2;

	if (!result) {
		try {
			String str2 = 1;
			result = 3;
			printf("exception not thrown or caught");
		} catch (const std::invalid_argument & e) { }
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_copyingString(void) {
	UNIT_TEST_START;
	int result = 0;

	String str = "hello world";
	char * cstr = str.cStringCopy();
	if (cstr == NULL) result = 1;
	BFFree(cstr);

	UNIT_TEST_END(!result, result);
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

	int max = 2 << 22;
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

		// test a non-digit case
		if (!result) {
			str = "one";
			val = String::toi(str);

			if (val != 0) {
				result = max;
			}
		}
	}

	UNIT_TEST_END(!result, result);
	return result;
}

int test_creatingstringfromformat() {
	UNIT_TEST_START;
	int result = 0;
	
	int max = 2 << 20;
	while (!result && max--) {
		const char * format = "%d tests left";
		String * str = String::createWithFormat(format, max);

		char buf[512];
		snprintf(buf, 512, format, max);

		if (str->compareString(buf)) {
			result = max;
		}

		BFRelease(str);
	}

	UNIT_TEST_END(!result, result);
	return result;
}

#define TEST_STRING_RANDOM_FILE "/tmp/string_tests_random_file"
#define TEST_STRING_RANDOM_TEXT "abcdefghijklmnopqrstuvwxz"
#define TEST_STRING_RANDOM_TEXT_OCC  (1024 * 20)

void TestStringRandomFileCreate() {
	FILE * f = fopen(TEST_STRING_RANDOM_FILE, "w");

	const char * buf = TEST_STRING_RANDOM_TEXT;
	int max = TEST_STRING_RANDOM_TEXT_OCC;
	while (max--) {
		fwrite(buf, sizeof(char), strlen(buf), f);
	}

	fclose(f);
}

void TestStringRandomFileDelete() {
	remove(TEST_STRING_RANDOM_FILE);
}

int test_readingFromFile() {
	UNIT_TEST_START;
	int result = 0;
	TestStringRandomFileCreate();
	
	String str;
	str.readFromFile(TEST_STRING_RANDOM_FILE);
	if (str.length() == 0) {
		result = 1;
	}

	TestStringRandomFileDelete();
	UNIT_TEST_END(!result, result);
	return result;
}

int test_data2string() {
	UNIT_TEST_START;
	int result = 0;
	int max = 2 << 16;
	const char * str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	size_t size = strlen(str);

	while (!result && max--) {
		Data d(size + 1, (const unsigned char *) str);
		String s(d);

		if (s.compareString(str)) {
			result = 1;
			break;
		}

		Data d0(size, (const unsigned char *) str);
		String s0(d0);
		if (s0.compareString(str)) {
			result = 2;
			break;
		}

		srand(time(0));
		size_t size = 2 << 9;
		unsigned char * buf = (unsigned char *) malloc(size);
		Data d1(size, buf);
		String s1(d1);
		
		// I am just making sure we aren't crashing with
		// a heap overflow
		//
		// if it doesn't crash, this should validate the
		// robustness of the Data to String conversion
		strlen(s1.cString());

		BFFree(buf);
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
	LAUNCH_TEST(test_nullstring, p, f);
	LAUNCH_TEST(test_copyingString, p, f);
	LAUNCH_TEST(test_addingCharacterToString, p, f);
	LAUNCH_TEST(test_clearstring, p, f);
	LAUNCH_TEST(test_removingCharacterFromString, p, f);
	LAUNCH_TEST(test_removingcharatindex, p, f);
	LAUNCH_TEST(test_indexingstring, p, f);
	LAUNCH_TEST(test_addandremove, p, f);
	LAUNCH_TEST(test_stringtoint, p, f);
	LAUNCH_TEST(test_creatingstringfromformat, p, f);
	LAUNCH_TEST(test_readingFromFile, p, f);
	LAUNCH_TEST(test_data2string, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // STRING_TESTS_HPP

