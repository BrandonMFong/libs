/**
 * author: brando
 * date: 11/22/24
 *
 * looks like I am going to implement the map in c. I am going to 
 * use a self balancing tree since search/insert/delete are O(logn)
 */

#ifndef MAP_H
#define MAP_H

#include "tree.h"

typedef void * BFMapKey;
typedef void * BFMapValue;

typedef struct BFMapKeyValuePair {
	BFMapKey key;
	BFMapValue value;
} BFMapKeyValuePair;

typedef struct BFMap {
	BFTree tree;
} BFMap;

/**
 * Caller owns memory
 */
BFMap * BFMapCreate();

/**
 * sets a compare callback that compares keys during our operations
 */
void BFMapSetCompare(BFMap * map, int (*compare)(BFMapKey a, BFMapKey b));

/**
 * releases map
 */
void BFMapRelease(BFMap * map);

/**
 * Adds entry for key and value
 */
int BFMapAdd(BFMap * map, BFMapKey key, BFMapValue value);

/**
 * returns value for key
 */
void * BFMapGetValue(BFMap * map, BFMapKey key);

/**
 * removes entry with key
 */
int BFMapRemove(BFMap * map, BFMapKey key);

#endif // MAP_H

