/**
 * author: Brando
 * date: 2/2/24
 */

#ifndef BF_OBJECT_TESTS_HPP
#define BF_OBJECT_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "object.hpp"
#include "release.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

int test_objectinit() {
	UNIT_TEST_START;
	int result = 0;

	Object o;

	if (Object::retainCount(o) != 1)
		result = 1;

	UNIT_TEST_END(!result, result);
	return result;
}

int test_objectretainer() {
	UNIT_TEST_START;
	int result = 0;

	Object * o = new Object;

	if (o == NULL)
		result = 1;
	else if (Object::retainCount(o) != 1)
		result = 2;

	int max = 2 << 8;
	while (!result && max) {
		srand(time(0));
		int retain = rand() % (2 << 16);

		for (int i = 0; i < retain; i++) {
			Object::retain(o);
		}

		if (Object::retainCount(o) != (retain + 1)) {
			result = max;
		}

		if (!result) {
			for (int i = 0; i < retain; i++) {
				BFRelease(o);
			}

			if (Object::retainCount(o) != 1) {
				result = max;
			}
		}

		max--;
	}

	if (!result) {
		BFRelease(o);
		int i = Object::retainCount(o);
		if (i) result = 4;
		else if (o) result = 5;
	}

	UNIT_TEST_END(!result, result);
	return result;
}

void object_tests(int * pass, int * fail) {
	int p = 0, f = 0;
	
	INTRO_TEST_FUNCTION;

	LAUNCH_TEST(test_objectinit, p, f);
	LAUNCH_TEST(test_objectretainer, p, f);

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // BF_OBJECT_TESTS_HPP

