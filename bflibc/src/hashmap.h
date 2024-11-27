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

/**
 * creates a hash map
 *
 * caller owns memory
 */
BFHashMap BFHashMapCreate();

/**
 * sets how key is hashed
 *
 * hash: maps a key to a unique integer. See hash.h to see availabled hash functions
 */
void BFHashMapSetHashFunction(BFHashMap map, unsigned long (*hash)(BFHashMapKey key));

/**
 * sets the compare callback. Used to overcome hash collisions
 */
void BFHashMapSetCompare(BFHashMap map, int (*compare)(BFHashMapKey a, BFHashMapKey b));

/**
 * sets how the keys and values are released
 */
void BFHashMapSetRelease(BFHashMap _map, void (*release)(BFHashMapKey key, BFHashMapValue value));

/**
 * releases map from memory
 */
void BFHashMapRelease(BFHashMap map);

/**
 * inserts key and value into hash map
 *
 * returns nonzero value on error
 */
int BFHashMapInsert(BFHashMap map, BFHashMapKey key, BFHashMapValue value);

/**
 * returns the number of key/value pairs there are in the map
 */
size_t BFHashMapGetSize(BFHashMap map);

/**
 * removes key/value pair for key
 *
 * returns nonzero value on error
 */
int BFHashMapRemove(BFHashMap map, BFHashMapKey key);

/**
 * Gets a value for key
 *
 * error: nonzero on error or if key/value pair couldn't be found
 */
BFHashMapValue BFHashMapGetValue(BFHashMap map, const BFHashMapKey key, int * error);

/**
 * returns true of key exists in map
 */
bool BFHashMapContains(BFHashMap map, const BFHashMapKey key);

#endif // HASH_MAP_H

