/**
 * author: Brando
 * date: 5/16/25
 */

#ifndef HASHSET_TESTS_H
#define HASHSET_TESTS_H

#include "clib_tests.h"
#include "hashset.h"
#include "hash.h"
#include <string.h>

unsigned long BFTestHashSetHashString(BFHashSetValue val) {
	return BFHashDjb2((unsigned char *) val);
}

int BFTestHashSetKeyCompare(BFHashSetValue aval, BFHashSetValue bval) {
	const char * a = (const char *) aval;
	const char * b = (const char *) bval;
	return strcmp(a, b);
}

char * _CreateRandomWord() {
	const char charset[] = "abcdefghijklmnopqrstuvwxyz";
	size_t charset_size = strlen(charset);

	srand(time(0));

	size_t reslen = 2 << 3;
	char * res = (char *) malloc(sizeof(char) * (reslen + 1));
	for (int i = 0; i < reslen; i++) {
		res[i] = charset[rand() % charset_size];
	}

	res[reslen] = '\0';

	return res;
}

BFTEST_UNIT_FUNC(test_hashsetinit, 2<<10, {
	BFHashSet set = BFHashSetCreate();
	BF_ASSERT(set, "null hash set");
	BFHashSetSetHashFunction(set, BFTestHashSetHashString);
	BFHashSetSetCompare(set, BFTestHashSetKeyCompare);
	BFHashSetSetRelease(set, free);
	BFHashSetRelease(set);
})

BFTEST_UNIT_FUNC(test_hashsetsize, 2<<10, {
	BFHashSet set = BFHashSetCreate();
	BF_ASSERT(set, "null set");

	BFHashSetSetHashFunction(set, BFTestHashSetHashString);
	BFHashSetSetCompare(set, BFTestHashSetKeyCompare);
	BFHashSetSetRelease(set, free);

	srand(time(0));
	int max = rand() % 2<<9;
	for (int i = 0; i < max; i++) {
		char * randword = _CreateRandomWord();
		BF_ASSERT(BFHashSetInsert(set, randword) == 0, "could not insert '%s'", randword);
	}

	BF_ASSERT(BFHashSetGetSize(set) == max, "size(%ld) != %ld", BFHashSetGetSize(set), max);
	BFHashSetRelease(set);
})

BFTEST_COVERAGE_FUNC(hashset_tests, {
	BFTEST_LAUNCH(test_hashsetinit);
	BFTEST_LAUNCH(test_hashsetsize);
})

#endif // HASHSET_TESTS_H

