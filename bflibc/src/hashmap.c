/**
 * author: brando
 * date: 11/25/24
 *
 * https://www.geeksforgeeks.org/implementation-of-hash-table-in-c-using-separate-chaining/
 */

#include "hashmap.h"
#include "free.h"
#include <stdlib.h>

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

void BFHashMapObjectRelease(BFHashMapObject * object) {
	BFFree(object);
}

typedef struct BFHashMapNode {
	BFHashMapObject * objects;
	size_t size;
} BFHashMapNode;

BFHashMapNode * BFHashMapNodeCreate() {
	BFHashMapNode * res = (BFHashMapNode *) malloc(sizeof(BFHashMapNode));
	res->objects = NULL;
	res->size = 0;
	return res;
}

void BFHashMapNodeRelease(BFHashMapNode * node) {
	BFFree(node);
}

typedef struct _BFHashMap {
	BFHashMapNode * nodes;
} _BFHashMap;

BFHashMap BFHashMapCreate() {
	_BFHashMap * res = (_BFHashMap *) malloc(sizeof(_BFHashMap));

	const size_t nodecount = 2 << 10;
	res->nodes = (BFHashMapNode *) malloc(sizeof(BFHashMapNode) * nodecount);

	return res;
}

void BFHashMapRelease(BFHashMap _map) {
	_BFHashMap * map = (_BFHashMap *) _map;
	BFFree(map->nodes);
	BFFree(map);
}

int BFHashMapInsert(BFHashMap map, BFHashMapKey key, BFHashMapValue value) {
	if (!map || !key || !value) {
		return -1;
	}
	
	return -1;
}

int BFHashMapRemove(BFHashMap map, BFHashMapKey key) {
	if (!map || !key) {
		return -1;
	}
	
	return -1;
}

BFHashMapValue BFHashMapGetValue(BFHashMap map, BFHashMapKey key) {
	if (!map || !key) {
		return NULL;
	}
	
	return NULL;
}

