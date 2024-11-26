/**
 * author: brando
 * date: 11/25/24
 *
 * https://www.geeksforgeeks.org/implementation-of-hash-table-in-c-using-separate-chaining/
 */

#include "hashmap.h"
#include "free.h"
#include <stdlib.h>
#include <string.h>

typedef struct BFHashMapObject {
	struct BFHashMapObject * next;
	BFHashMapKey key;
	BFHashMapValue value;
} BFHashMapObject;

BFHashMapObject * BFHashMapObjectCreate() {
	BFHashMapObject * res = (BFHashMapObject *) malloc(sizeof(BFHashMapObject));
	res->next = NULL;
	res->key = 0;
	res->value = 0;
	return res;
}

void BFHashMapObjectRelease(
	BFHashMapObject * object,
	void (*release)(BFHashMapKey key, BFHashMapValue value)
) {
	if (!object) return;
	
	object->next = NULL;
	
	if (release) {
		release(object->key, object->value);
	} else {
		object->key = 0;
		object->value = 0;
	}

	BFFree(object);
}

/**
 * traverse through list and releases from end to start
 */
void BFHashMapObjectReleaseTraverse(
	BFHashMapObject * object,
	void (*release)(BFHashMapKey key, BFHashMapValue value)
) {
	if (!object) return;
	BFHashMapObjectReleaseTraverse(object->next, release);
	BFHashMapObjectRelease(object, release);
}

typedef struct BFHashMapNode {
	BFHashMapObject * first;
	size_t size;
} BFHashMapNode;

int BFHashMapNodeSetKeyValue(BFHashMapNode * node, BFHashMapKey key, BFHashMapValue value) {
	if (!node) {
		return -1;
	}

	// create object
	BFHashMapObject * object = BFHashMapObjectCreate();
	object->key = key;
	object->value = value;

	// insert into list	
	if (!node->first) { // empty list
		node->first = object;
	} else { // non-empty list
		BFHashMapObject * obj = node->first;
		while (obj->next) { obj = obj->next; }
		obj->next = object;
	}

	node->size++;

	return 0;
}

int BFHashMapNodeRemoveKeyValue(
	BFHashMapNode * node,
	BFHashMapKey key,
	int (*compare)(BFHashMapKey akey, BFHashMapKey bkey),
	void (*release)(BFHashMapKey key, BFHashMapValue value)
) {
	if (!node) {
		return -1;
	}

	BFHashMapObject * object = node->first;
	BFHashMapObject * prev = NULL;

	// delete head
	if (compare(node->first->key, key) == 0) {
		node->first = object->next;
		BFHashMapObjectRelease(object, release);
		node->size--;
		return 0;
	}

	// if not at head, then go to the next
	prev = object;
	object = object->next;

	while (object) {
		if (compare(object->key, key) == 0) {
			prev->next = object->next;
			BFHashMapObjectRelease(object, release);
			break;
		}
		prev = object;
		object = object->next;
	}

	node->size--;

	return 0;
}

typedef struct _BFHashMap {
	BFHashMapNode * nodes;

	/**
	 * size of nodes array above
	 */
	size_t nodeListSize;

	/**
	 * number of key/value pairs
	 */
	size_t count;

	/**
	 * the hash function used to index nodes
	 */
	unsigned long (*hash)(BFHashMapKey key);

	/**
	 * defines how keys and values memory are released
	 */
	void (*release)(BFHashMapKey key, BFHashMapValue value);

	/**
	 * used to compare keys when caller wants a value for key. see 
	 * `BFHashMapRelease`
	 *
	 * akey < bkey, return() < 0
	 * akey > bkey, return() > 0
	 * akey == bkey, return() == 0
	 */
	int (*compare)(BFHashMapKey akey, BFHashMapKey bkey);
} _BFHashMap;

BFHashMap BFHashMapCreate() {
	_BFHashMap * res = (_BFHashMap *) malloc(sizeof(_BFHashMap));

	res->nodeListSize = 2 << 10;
	res->nodes = (BFHashMapNode *) malloc(sizeof(BFHashMapNode) * res->nodeListSize);
	memset(res->nodes, 0, sizeof(BFHashMapNode) * res->nodeListSize);
	res->hash = NULL;
	res->release = NULL;
	res->compare = NULL;
	res->count = 0;

	return res;
}

void BFHashMapSetHashFunction(BFHashMap _map, unsigned long (*hash)(BFHashMapKey key)) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map) return;
	map->hash = hash;
}

void BFHashMapSetCompare(BFHashMap _map, int (*compare)(BFHashMapKey a, BFHashMapKey b)) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map) return;
	map->compare = compare;
}

void BFHashMapRelease(BFHashMap _map) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map) return;

	// go through each node and release memory
	for (int i = 0; i < map->nodeListSize; i++) {
		BFHashMapObjectReleaseTraverse(map->nodes[i].first, map->release);
		map->nodes[i].size = 0;
	}
	BFFree(map->nodes);
	BFFree(map);
}

size_t BFHashMapGetSize(BFHashMap _map) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map) {
		return 0;
	}
	return map->count;
}

int BFHashMapInsert(BFHashMap _map, BFHashMapKey key, BFHashMapValue value) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map || !map->hash) {
		return -1;
	}

	// get index using hash
	unsigned long index = map->hash(key) % map->nodeListSize;
	
	// get node
	BFHashMapNode node = map->nodes[index];

	// set key and value
	int err = BFHashMapNodeSetKeyValue(&node, key, value);
	if (err != 0) {
		return err;
	}

	// save node
	map->nodes[index] = node;
	map->count++;

	return 0;
}

int BFHashMapRemove(BFHashMap _map, BFHashMapKey key) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map || !map->compare) {
		return -1;
	}
		
	// get index using hash
	unsigned long index = map->hash(key) % map->nodeListSize;
	
	// get node
	BFHashMapNode node = map->nodes[index];
	
	int err = BFHashMapNodeRemoveKeyValue(&node, key, map->compare, map->release);
	if (err != 0) {
		return err;
	}

	// save node
	map->nodes[index] = node;
	map->count--;

	return 0;
}

bool BFHashMapContains(BFHashMap _map, const BFHashMapKey key) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map || !map->compare) {
		return false;
	}
	
	// get index using hash
	unsigned long index = map->hash(key) % map->nodeListSize;
	
	// get node
	BFHashMapNode node = map->nodes[index];

	// get the object for key
	BFHashMapObject * object = node.first;
	while (object) {
		if (map->compare(object->key, key) == 0) {
			return true;
		}
		object = object->next;
	}
	
	return false;
}

BFHashMapValue BFHashMapGetValue(BFHashMap _map, const BFHashMapKey key, int * error) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map || !map->compare) {
		*error = -1;
		return NULL;
	}
	
	// get index using hash
	unsigned long index = map->hash(key) % map->nodeListSize;
	
	// get node
	BFHashMapNode node = map->nodes[index];

	// get the object for key
	BFHashMapObject * object = node.first;
	while (object) {
		if (map->compare(object->key, key) == 0) {
			return object->value;
		}
		object = object->next;
	}

	// value could not be found here
	*error = -1;
	return NULL;
}

