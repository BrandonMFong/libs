/**
 * author: brando
 * date: 11/7/24
 */

#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "object.hpp"

namespace BF {

/**
 * This class serves as a base class to all array type
 * implementations. Purpose is to abstract each class such
 * that I can have a set of mutable functions like sorting
 * to be used across different classes
 */
class Collection : public Object {
public:
	~Collection();
protected:
	Collection();
};
}

#endif // COLLECTION_HPP

