/**
 * author: Brando
 * date: 2/2/24
 */

#ifndef BF_OBJECT_TESTS_HPP
#define BF_OBJECT_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "object.hpp"
#include "release.hpp"
#include "retain.hpp"

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF;

//int test_objectinit() {
BFTEST_UNIT_FUNC(test_objectinit, 1,  {
	Object o;

	if (Object::retainCount(o) != 1)
		result = 1;
})

//int test_objectretainer() {
BFTEST_UNIT_FUNC(test_objectretainer, 1,  {
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
			BFRetain(o);
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
})

//int test_objectshallowcopy() {
BFTEST_UNIT_FUNC(test_objectshallowcopy, 1,  {
	Object o;

	if (Object::retainCount(o) != 1)
		result = 1;

	if (!result) {
		Object * so = new Object(o);
		if (so == NULL) result = 2;
		else if (Object::retainCount(so) != 1) result = 3;
		else if (so->_lock == o._lock) result = 4;
	}
})

BFTEST_COVERAGE_FUNC(object_tests, {
	BFTEST_LAUNCH(test_objectinit);
	BFTEST_LAUNCH(test_objectretainer);
	BFTEST_LAUNCH(test_objectshallowcopy);
})

#endif // BF_OBJECT_TESTS_HPP

