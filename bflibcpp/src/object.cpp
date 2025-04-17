/**
 * author: brando
 * date: 2/2/24
 */

#include "object.hpp"

using namespace BF;

const char * Object::className() const {
	return "BF::Object";
}

Object::Object() {
	this->_retainCount = 1;
	BFLockCreate(&this->_lock);
}

/** 
 * if someone is doing a shallow copy of 
 * this object, I want to make sure
 * the new copy has their own lock and
 * a retain count that is reset
 */
Object::Object(Object & obj) : Object() {}

Object::~Object() {
	this->_retainCount = 0;
	BFLockDestroy(&this->_lock);
}

void Object::retain(const Object & obj) {
	Object::retain((Object &) obj);
}

void Object::retain(Object & obj) {
	BFLockLock(&obj._lock);
	obj._retainCount++;
	BFLockUnlock(&obj._lock);
}

void Object::retain(const Object * obj) {
	Object::retain((Object *) obj);
}

void Object::retain(Object * obj) {
	if (obj) {
		Object::retain(*obj);
	}
}

void Object::release(const Object & obj) {
	Object::release((Object &) obj);
}

void Object::release(Object & obj) {
	BFLockLock(&obj._lock);
	obj._retainCount--;
	BFLockUnlock(&obj._lock);
}

void Object::release(const Object * obj) {
	Object::release((Object *) obj);
}

void Object::release(Object * obj) {
	if (obj) {
		BFLockLock(&obj->_lock);
		obj->_retainCount--;

		if (obj->_retainCount <= 0) {
			BFLockUnlock(&obj->_lock);
			delete obj;
		} else {
			BFLockUnlock(&obj->_lock);
		}
	}
}

int Object::retainCount(const Object * obj) {
	return Object::retainCount((Object *) obj);
}

int Object::retainCount(Object * obj) {
	if (obj) {
		BFLockLock(&obj->_lock);
		int result = obj->_retainCount;
		BFLockUnlock(&obj->_lock);
		return result;
	}
	return 0;
}

int Object::retainCount(const Object & obj) {
	return Object::retainCount((Object &) obj);
}

int Object::retainCount(Object & obj) {
	return Object::retainCount(&obj);
}

bool Object::operator==(int num)  const { return false; }
bool Object::operator!=(int num)  const { return *this == num; }
void Object::operator=(int num)  const { }

int Object::compare(const Object & obj) const {
	return 0;
}

