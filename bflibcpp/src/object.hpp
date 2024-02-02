/**
 * author: brando
 * date: 2/2/24
 */

#ifndef BF_OBJECT_HPP
#define BF_OBJECT_HPP

namespace BF {

class Object {
public:
	Object();
	virtual ~Object();

	static void retain(Object * obj);
	static void release(Object * obj);

	int retainCount() const;

private:
	int _retainCount;
};

}

#endif // BF_OBJECT_HPP

