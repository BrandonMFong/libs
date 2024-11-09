/**
 * author: brando
 * date: 11/8/24
 */

#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "object.hpp"

namespace BF {

template <typename S = size_t>
class Collection : public Object {
public:
	virtual ~Collection() { }
	
	virtual S size() const = 0;

protected:
	Collection() : Object() { }
};

}
#endif // COLLECTION_HPP

