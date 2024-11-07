/**
 * author: brando
 * date: 11/7/24
 */

#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "object.hpp"

namespace BF {

typedef enum {
	kCollectionSortBubble = 1,
	kCollectionSortInsertion = 2,
	kCollectionSortSelection = 3,
	kCollectionSortMerge = 4,
	kCollectionSortQuick = 5,
	kCollectionSortRadix = 6,
} CollectionSort;

/**
 * This class serves as a base class to all array type
 * implementations. Purpose is to abstract each class such
 * that I can have a set of mutable functions like sorting
 * to be used across different classes
 */
template <typename T, typename S = size_t>
class Collection : public Object {
public:
	virtual ~Collection() {

	}

	int sort(CollectionSort type = kCollectionSortMerge) {
		switch (type) {
			case kCollectionSortMerge:
			default:
				break;
		}
		return 0;
	}
protected:
	Collection() : Object() { }
};
}

#endif // COLLECTION_HPP

