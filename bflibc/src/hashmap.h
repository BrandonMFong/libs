/**
 * author: brando
 * date: 11/25/24
 *
 * a hash map will have an array of nodes. Each
 * node will have a linked list of objects that
 * handles hash collisions
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef void * BFHashMap;
typedef void * BFHashMapKey;
typedef void * BFHashMapValue;

BFHashMap BFHashMapCreate();
void BFHashMapSetHashFunction(BFHashMap map, unsigned long (*hash)(BFHashMapKey key));
void BFHashMapSetCompare(BFHashMap map, int (*compare)(BFHashMapKey a, BFHashMapKey b));
void BFHashMapRelease(BFHashMap map);
int BFHashMapInsert(BFHashMap map, BFHashMapKey key, BFHashMapValue value);
size_t BFHashMapGetSize(BFHashMap map);
int BFHashMapRemove(BFHashMap map, BFHashMapKey key);
BFHashMapValue BFHashMapGetValue(BFHashMap map, const BFHashMapKey key, int * error);
bool BFHashMapContains(BFHashMap map, const BFHashMapKey key);

#endif // HASH_MAP_H

