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

typedef void * BFHashMap;
typedef void * BFHashMapKey;
typedef void * BFHashMapValue;

BFHashMap BFHashMapCreate();
void BFHashMapSetHashFunction(BFHashMap map, unsigned long (*hash)(BFHashMapKey key));
void BFHashMapRelease(BFHashMap map);
int BFHashMapInsert(BFHashMap map, BFHashMapKey key, BFHashMapValue value);
int BFHashMapRemove(BFHashMap map, BFHashMapKey key);
BFHashMapValue BFHashMapGetValue(BFHashMap map, BFHashMapKey key);

#endif // HASH_MAP_H

