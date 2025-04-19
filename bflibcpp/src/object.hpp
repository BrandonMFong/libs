/**
 * author: brando
 * date: 2/2/24
 */

#ifndef BF_OBJECT_HPP
#define BF_OBJECT_HPP

extern "C" {
#include <bflibc/lock.h>
}

namespace BF {

/**
 * Base class Object is specifically designed to imitate Apple's
 * Core Foundation memory management.  Every class that adopts this
 * object will have a memory retaining counter. Owner of class can choose
 * to manually release memory with `delete` or to use `Object::release()`.
 */
class Object {
public:
	Object();
	Object(Object & obj);
	virtual ~Object();

	virtual const char * className() const;

	/**
	 * increments retain count by 1
	 */
	static void retain(Object * obj);
	static void retain(Object & obj);
	static void retain(const Object * obj);
	static void retain(const Object & obj);

	/**
	 * decrements retain count by 1
	 *
	 * if retain count reaches 0, this function
	 * will delete `obj`.  Using `obj` after will
	 * incur undefined behavior
	 */
	static void release(Object * obj);
	static void release(Object & obj);
	static void release(const Object * obj);
	static void release(const Object & obj);

	static int retainCount(Object * obj);
	static int retainCount(const Object * obj);
	static int retainCount(Object & obj);
	static int retainCount(const Object & obj);

	/**
	 * acts like memcmp, same return values
	 *
	 * this is optional for subclasses
	 *
	 * this class will not have any implmentation and will always
	 * return 0
	 */
	virtual int compare(const Object & obj) const;

private:
	int _retainCount;

	BFLock _lock;

public:

	bool operator==(int num) const;
	bool operator!=(int num) const;
};

}

#endif // BF_OBJECT_HPP

