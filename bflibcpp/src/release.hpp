/**
 * author: brando
 * date: 2/2/24
 */

#ifndef RELEASE_HPP
#define RELEASE_HPP

#include "object.hpp"

#define BFRelease(x) if (x != 0) {\
	int rc = BF::Object::retainCount(x);\
	BF::Object::release(x);\
	if (rc == 1) x = 0;\
}\

#endif // RELEASE_HPP

