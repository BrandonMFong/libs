/**
 * author: Brando
 * date: 11/20/24
 */

#ifndef HASH_TESTS_H
#define HASH_TESTS_H

#include "clib_tests.h"
#include "hash.h"
#include "tree.h"

int BFHashTreeCompare(BFTreeNodeObject aobj, BFTreeNodeObject bobj) {
	unsigned long a = (unsigned long) aobj;
	unsigned long b = (unsigned long) bobj;
	return a - b;
}

BFTEST_UNIT_FUNC(test_djb2, 1, {
	BFTree hashes = BFTreeCreate();
	BF_ASSERT(hashes, "tree null");
	BFTreeSetCompare(hashes, BFHashTreeCompare);

	char text[2<<8];
	strcpy(text, "Lorem ipsum dolor sit amet consectetur adipiscing elit sed do eiusmod tempor incididunt ut labore et dolore magna aliqua Excepteur sint occaecat cupidatat non proident sunt in culpa qui officia deserunt mollit anim id est laborum");
	char * token = strtok(text, " ");
	int collisions = 0;
	while (token != NULL) {
		unsigned long hash = BFHashDjb2((unsigned char *) token);
		if (BFTreeContains(hashes, (BFTreeNodeObject) hash)) {
			collisions++;
		} else {
			int err = BFTreeInsert(hashes, (BFTreeNodeObject) hash);
			BF_ASSERT(err == 0, "error inserting %d", err);
		}
		token = strtok(NULL, " ");
	}

	BFTreeRelease(hashes);
	BF_ASSERT(collisions == 0, "there were %d collisions", collisions);
})

BFTEST_COVERAGE_FUNC(hash_tests, {
	BFTEST_LAUNCH(test_djb2);
})

#endif // HASH_TESTS_H

