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

/**
 * traverse through list and releases from end to start
 */
void BFHashMapObjectRelease(
	BFHashMapObject * object,
	void (*release)(BFHashMapKey key, BFHashMapValue value)
) {
	if (!object) return;
	BFHashMapObjectRelease(object->next, release);
	object->next = NULL;
	
	if (release) {
		release(object->key, object->value);
	}
	
	BFFree(object);
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

typedef struct _BFHashMap {
	BFHashMapNode * nodes;

	/**
	 * size of nodes array above
	 */
	size_t size;

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

	res->size = 2 << 10;
	res->nodes = (BFHashMapNode *) malloc(sizeof(BFHashMapNode) * res->size);
	memset(res->nodes, 0, sizeof(BFHashMapNode) * res->size);
	res->hash = NULL;
	res->release = NULL;
	res->compare = NULL;

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
	for (int i = 0; i < map->size; i++) {
		BFHashMapObjectRelease(map->nodes[i].first, map->release);
		map->nodes[i].size = 0;
	}
	BFFree(map->nodes);
	BFFree(map);
}

int BFHashMapInsert(BFHashMap _map, BFHashMapKey key, BFHashMapValue value) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map || !map->hash) {
		return -1;
	}

	// get index using hash
	unsigned long index = map->hash(key) % map->size;
	
	// get node
	BFHashMapNode node = map->nodes[index];

	// set key and value
	int err = BFHashMapNodeSetKeyValue(&node, key, value);
	if (err != 0) {
		return err;
	}

	// save node
	map->nodes[index] = node;

	return 0;
}

int BFHashMapRemove(BFHashMap map, BFHashMapKey key) {
	if (!map) {
		return -1;
	}
	
	return -1;
}

BFHashMapValue BFHashMapGetValue(BFHashMap _map, BFHashMapKey key) {
	_BFHashMap * map = (_BFHashMap *) _map;
	if (!map || !map->compare) {
		return NULL;
	}
	
	// get index using hash
	unsigned long index = map->hash(key) % map->size;
	
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

	return NULL;
}

