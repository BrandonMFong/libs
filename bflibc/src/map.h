/**
 * author: brando
 * date: 11/22/24
 *
 * looks like I am going to implement the map in c. I am going to 
 * use a self balancing tree since search/insert/delete are O(logn)
 */

#ifndef MAP_H
#define MAP_H

typedef void * BFMap;

/**
 * Caller owns memory
 */
BFMap BFMapCreate();

/**
 * releases map
 */
void BFMapRelease(BFMap map);

/**
 * Adds entry for key and value
 */
int BFMapAdd(BFMap * map, void * key, void * value);

/**
 * returns value for key
 */
void * BFMapGetValue(BFMap * map, void * key);

/**
 * removes entry with key
 */
int BFMapRemove(BFMap * map, void * key);

#endif // MAP_H

