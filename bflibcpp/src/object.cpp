/**
 * author: brando
 * date: 2/2/24
 */

#include "object.hpp"

using namespace BF;

Object::Object() {
	this->_retainCount = 0;
}

Object::~Object() {

}

void Object::retain(Object & obj) {
	obj._retainCount++;
}

void Object::release(Object & obj) {
	obj._retainCount--;
}

