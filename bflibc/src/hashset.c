/**
 * author: brando
 * date: 5/14/25
 */

#include "hashset.h"
#include "hashmap.h"
#include "free.h"

typedef struct _BFHashSet {
	/**
	 * we will use a hash map with this schema:
	 * <key>:<value> -> <hash set value>:<hash set object>
	 */
	BFHashMap map;

	void (*release)(BFHashSetValue value);
} _BFHashSet;

/**
 * value: hashset object
 * key: hashset value
 */
void _BFHashSetReleaseHashMap(BFHashSetValue value, BFHashSet _set) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set && !set->release) return;

	set->release(value);
}

BFHashSet BFHashSetCreate() {
	_BFHashSet * res = (_BFHashSet *) malloc(sizeof(_BFHashSet));
	if (!res) return NULL;

	res->release = NULL;

	res->map = BFHashMapCreate();
	if (!res->map) {
		return NULL;
	}
	
	BFHashMapSetRelease(res->map, _BFHashSetReleaseHashMap);

	return (BFHashSet) res;
}

void BFHashSetSetCompare(BFHashSet _set, int (*compare)(BFHashSetValue a, BFHashSetValue b)) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	BFHashMapSetCompare(set->map, compare);
}

void BFHashSetSetHashFunction(BFHashSet _set, unsigned long (*hash)(BFHashSetValue value)) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	BFHashMapSetHashFunction(set->map, hash);
}

void BFHashSetSetRelease(BFHashSet _set, void (*release)(BFHashSetValue value)) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	set->release = release;
}

void BFHashSetRelease(BFHashSet _set) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return;

	BFHashMapRelease(set->map);
	BFFree(set);
}

int BFHashSetInsert(BFHashSet _set, BFHashSetValue value) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return -1;

	return BFHashMapInsert(set->map, value, set);
}

int BFHashSetRemove(BFHashSet _set, BFHashSetValue value) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return -1;

	return BFHashMapRemove(set->map, value);
}

bool BFHashSetContains(BFHashSet _set, BFHashSetValue value) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return false;

	return BFHashMapContains(set->map, value);
}

size_t BFHashSetGetSize(BFHashSet _set) {
	_BFHashSet * set = (_BFHashSet *) _set;
	if (!set) return 0;

	return BFHashMapGetSize(set->map);
}

