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

typedef void * BFMap;

typedef void * BFMapKey;
typedef void * BFMapValue;

/**
 * a pair object, contain key and value of any type
 *
 * use the accessors below to access data
 */
typedef void * BFMapKeyValuePair;
BFMapKey BFMapKeyValuePairGetKey(BFMapKeyValuePair);
BFMapValue BFMapKeyValuePairGetValue(BFMapKeyValuePair);

/**
 * Caller owns memory
 */
BFMap BFMapCreate();

/**
 * sets a compare callback that compares keys during our operations
 */
void BFMapSetCompare(BFMap map, int (*compare)(BFMapKey a, BFMapKey b));

/**
 * defines how key and value are released when BFMapRemove is called
 */
void BFMapSetRelease(BFMap map, void (*release)(BFMapKey key, BFMapValue value));

/**
 * releases map
 */
void BFMapRelease(BFMap map);

/**
 * Adds entry for key and value
 */
int BFMapInsert(BFMap map, BFMapKey key, BFMapValue value);

/**
 * returns value for key
 */
void * BFMapGetValue(BFMap map, BFMapKey key);

/**
 * removes entry with key
 */
int BFMapRemove(BFMap map, BFMapKey key);

#endif // MAP_H

