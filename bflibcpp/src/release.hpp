/**
 * author: brando
 * date: 2/2/24
 */

#ifndef RELEASE_HPP
#define RELEASE_HPP

#include "object.hpp"
#include <stdbool.h>

static bool _BFReleaseObjectIsPointer(const BF::Object * object) { return true; }
static bool _BFReleaseObjectIsPointer(const BF::Object & object) { return false; }

#define BFRelease(obj) \
	if (_BFReleaseObjectIsPointer(obj)) {\
		if (obj != 0) {\
			int rc = BF::Object::retainCount(obj);\
			BF::Object::release(obj);\
			if (rc == 1) obj = 0;\
		}\
	}\

#endif // RELEASE_HPP

