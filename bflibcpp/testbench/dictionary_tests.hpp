/**
 * author: Brando
 * date: 10/4/22
 */

#ifndef DICTIONARY_TESTS_HPP
#define DICTIONARY_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include <dictionary.hpp>
#include <string.hpp>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_DictionaryInit() {
	int result = 0;

	Dictionary<char *, char *> d;

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_DictionarySize() {
	int result = 0;

	Dictionary<char *, char *> d;

	if (d.size() != 0) {
		result = 1;
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_AddingKeyValues() {
	int result = 0;

	Dictionary<const char *, int> d;
	
	result = d.setValueForKey("Hello", 1);

	if (!result) {
		result = d.setValueForKey("World", 2);
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_GettingValueForKey() {
	int result = 0;

	Dictionary<const char *, int> d;
	
	result = d.setValueForKey("Hello", 1);

	if (!result) {
		result = d.setValueForKey("World", 2);
	}

	if (!result) {
		int val = d.valueForKey("Hello");

		if (val != 1) {
			result = 2;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int KeyStringCompare(const char * k1, const char * k2) {
	int res = strcmp(k1, k2);
	if (res < 0) return -1;
	else if (res > 0) return 1;
	else return 0;
}

void KeyStringValueStringRetain(const char ** key, const char ** value) {
	if (key) {
		const char * k = *key;
		*key = BFStringCopyString(k);
	}

	if (value) {
		const char * v = *value;
		*value = BFStringCopyString(v);
	}
}

void KeyStringValueStringRelease(const char ** key, const char ** value) {
	if (key) {
		char * k = (char *) *key;
		Delete(k);
	}

	if (value) {
		const char * v = (char *) *value;
		Delete(v);
	}
}

int test_DictionaryRetainAndReleaseCallbacks() {
	int result = 0;

	Dictionary<const char *, const char *> d(KeyStringCompare, KeyStringValueStringRetain, KeyStringValueStringRelease);

	result = d.setValueForKey("one", "1");

	if (!result) {
		result = d.setValueForKey("two", "2");
	}

	if (!result) {
		result = d.setValueForKey("three", "3");
	}

	if (!result) {
		result = d.setValueForKey("four", "4");
	}

	if (!result) {
		result = d.setValueForKey("five", "5");
	}

	if (!result) {
		const char * val = d.valueForKey("four");
		if (val == NULL) {
			result = 3;
			printf("Val is null\n");
		} else if (strcmp(val, "4")) {
			printf("val is %s\n", val);
			result = 4;
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

int test_RemovingEntryFromDictionary() {
	int result = 0;

	Dictionary<const char *, const char *> d;
	d.setEntryReleaseCallback(KeyStringValueStringRelease);
	d.setEntryRetainCallback(KeyStringValueStringRetain);
	d.setKeyCompareCallback(KeyStringCompare);

	result = d.setValueForKey("one", "1");

	if (!result) {
		result = d.setValueForKey("two", "2");
	}

	if (!result) {
		result = d.setValueForKey("three", "3");
	}

	if (!result) {
		result = d.setValueForKey("four", "4");
	}

	if (!result) {
		result = d.setValueForKey("five", "5");
	}

	if (!result) {
		result = d.removeValueForKey("four");
	}

	if (!result) {
		const char * val = d.valueForKey("four");
		if (val != NULL) {
			result = 3;
			printf("Val should be null\n");
		}
	}

	PRINT_TEST_RESULTS(!result);
	return result;
}

#define TEST_DICTIONARY_PRINT 0
#if TEST_DICTIONARY_PRINT == 1
int test_DictionaryPrint() {
	int result = 0;

	Dictionary<const char *, const char *> d;
	d.setEntryReleaseCallback(KeyStringValueStringRelease);
	d.setEntryRetainCallback(KeyStringValueStringRetain);
	d.setKeyCompareCallback(KeyStringCompare);

	result = d.setValueForKey("one", "1");

	if (!result) {
		result = d.setValueForKey("two", "2");
	}

	if (!result) {
		result = d.setValueForKey("three", "3");
	}

	if (!result) {
		result = d.setValueForKey("four", "4");
	}

	if (!result) {
		result = d.setValueForKey("five", "5");
	}

	if (!result)
		d.print();

	PRINT_TEST_RESULTS(!result);
	return result;
}
#endif

int KeyLibStringCompare(String k1, String k2) {
	if (k1 < k2) return -1;
	else if (k1 > k2) return 1;
	else return 0;
}

bool DictionaryTestStringArrayContainsString(const char ** arr, int size, const char * str) {
	for (int i = 0; i < size; i++)
		if (!strcmp(arr[i], str)) return true;
	return false;
}

int test_TraversingThroughDictionary() {
	int result = 0;

	Dictionary<String, String> d;
	d.setKeyCompareCallback(KeyLibStringCompare);

	const int size = 5;
	const char * keys[size] = {"one", "two", "three", "four", "five"};
	const char * values[size] = {"1", "2", "3", "4", "5"};
	
	for (int i = 0; i < size; i++) {
		d.setValueForKey(keys[i], values[i]);
	}

	Dictionary<String, String>::Iterator * itr = 0;
	result = d.createIterator(&itr);

	Dictionary<String, String>::Entry * ent = 0;
	int i = 0;
	while (!result && !itr->finished() && (i < size)) {
		ent = itr->current();

		// Make sure we actually have those keys and values
		if (!DictionaryTestStringArrayContainsString(keys, size, ent->key())) result = 11;
		else if (!DictionaryTestStringArrayContainsString(values, size, ent->value())) result = 12;
		else {
			result = itr->next();
			i++;
		}
	}

	Delete(itr);

	PRINT_TEST_RESULTS(!result);
	return result;
}

void dictionary_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_DictionaryInit()) p++;
	else f++;

	if (!test_DictionarySize()) p++;
	else f++;

	if (!test_AddingKeyValues()) p++;
	else f++;

	if (!test_GettingValueForKey()) p++;
	else f++;

	if (!test_DictionaryRetainAndReleaseCallbacks()) p++;
	else f++;

	if (!test_RemovingEntryFromDictionary()) p++;
	else f++;

#if TEST_DICTIONARY_PRINT == 1
	if (!test_DictionaryPrint()) p++;
	else f++;
#endif

	if (!test_TraversingThroughDictionary()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // DICTIONARY_TESTS_HPP

