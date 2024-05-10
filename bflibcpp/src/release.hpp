/**
 * author: brando
 * date: 2/2/24
 */

#ifndef RELEASE_HPP
#define RELEASE_HPP

#include "object.hpp"

#define BFRelease(x) if (x != 0) {\
	BF::Object * obj = (BF::Object *) x;\
	int rc = BF::Object::retainCount(obj);\
	BF::Object::release(obj);\
	if (rc == 1) x = 0;\
}\

#endif // RELEASE_HPP

