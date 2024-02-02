/**
 * author: brando
 * date: 2/2/24
 */

#include "object.hpp"
#include "delete.hpp"

using namespace BF;

Object::Object() {
	this->_retainCount = 1;
}

Object::~Object() {
	this->_retainCount = 0;
}

void Object::retain(Object * obj) {
	if (obj)
		obj->_retainCount++;
}

void Object::release(Object * obj) {
	if (obj) {
		obj->_retainCount--;

		if (obj->_retainCount == 0) {
			Delete(obj);
		}
	}
}

int Object::retainCount() const {
	return this->_retainCount;
}

