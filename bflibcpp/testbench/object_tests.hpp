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

BFTEST_UNIT_FUNC(test_objectinit, 2<<10,  {
	Object o;
	BF_ASSERT(Object::retainCount(o) == 1);
	Object::retain(o);
	BF_ASSERT(Object::retainCount(o) == 2);
	Object::release(o);
})

BFTEST_UNIT_FUNC(test_objectretainer, 2<<8,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		srand(time(0));
	}
	int retain = rand() % (2 << 16);

	Object * o = new Object;

	BF_ASSERT(o != NULL);
	BF_ASSERT(Object::retainCount(o) == 1);

	for (int i = 0; i < retain; i++) {
		BFRetain(o);
	}

	BF_ASSERT(Object::retainCount(o) == (retain + 1));

	for (int i = 0; i < retain; i++) {
		BFRelease(o);
	}

	BF_ASSERT(Object::retainCount(o) == 1);

	BFRelease(o);
	
	int i = Object::retainCount(o);
	BF_ASSERT(i == 0);
	BF_ASSERT(o == NULL);
})

BFTEST_UNIT_FUNC(test_constObjectretainer, 2<<8,  {
	if (BFTEST_UNIT_FUNC_ITR == 0) {
		srand(time(0));
	}
	int retain = rand() % (2 << 16);

	const Object * o = new Object;

	BF_ASSERT(o != NULL);
	BF_ASSERT(Object::retainCount(o) == 1);

	for (int i = 0; i < retain; i++) {
		BFRetain(o);
	}

	BF_ASSERT(Object::retainCount(o) == (retain + 1));

	for (int i = 0; i < retain; i++) {
		BFRelease(o);
	}

	BF_ASSERT(Object::retainCount(o) == 1);

	BFRelease(o);
	
	int i = Object::retainCount(o);
	BF_ASSERT(i == 0);
	BF_ASSERT(o == NULL);
})

BFTEST_UNIT_FUNC(test_objectshallowcopy, 2<<12,  {
	Object o;

	BF_ASSERT(Object::retainCount(o) == 1);
	
	Object * so = new Object(o);
	BF_ASSERT(so != NULL);
	BF_ASSERT(Object::retainCount(so) == 1);
	BF_ASSERT(so->_lock != o._lock);

	BFRelease(so);
})

BFTEST_COVERAGE_FUNC(object_tests, {
	BFTEST_LAUNCH(test_objectinit);
	BFTEST_LAUNCH(test_objectretainer);
	BFTEST_LAUNCH(test_objectshallowcopy);
	BFTEST_LAUNCH(test_constObjectretainer);

})

#endif // BF_OBJECT_TESTS_HPP

