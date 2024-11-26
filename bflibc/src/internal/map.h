/**
 * author: brando
 * date: 11/25/24
 */

#ifndef INTERNAL_MAP_H
#define INTERNAL_MAP_H

#include "../tree.h"

typedef struct _BFMapKeyValuePair {
	BFMapKey key;
	BFMapValue value;

	// releases key and value
	void (*release)(BFMapKey key, BFMapValue value);
	int (*compare)(BFMapKey a, BFMapKey b);
} _BFMapKeyValuePair;

typedef struct _BFMap {
	BFTree tree;
	
	// releases key and value
	void (*release)(BFMapKey key, BFMapValue value);

	// defines how the tree's compare callback
	// we keep a copy for ourselves when we traverse
	// the tree
	int (*compare)(BFMapKey a, BFMapKey b);
} _BFMap;

#endif // INTERNAL_MAP_H

