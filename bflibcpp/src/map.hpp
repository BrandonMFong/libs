/**
 * author: brando
 * date: 11/26/24
 */

#ifndef MAP_HPP
#define MAP_HPP

#include "basicmap.hpp"
#include "release.hpp"

extern "C" {
#include <bflibc/map.h>
}

namespace BF {

/**
 * Map implemented using self-balancing tree. See bflibc/map.h
 */
template <typename K, typename V, typename S = size_t>
class Map : public BasicMap<K,V,S> {
public:
	Map() : _map(NULL), BasicMap<K,V,S>() {
		this->_map = BFMapCreate();
		if (!this->_map) return;
		BFMapSetCompare(this->_map, this->_BFMapCompare);
		BFMapSetRelease(this->_map, this->_BFMapRelease);
	}

	virtual ~Map() {
		BFMapRelease(this->_map);
	}

private:
	virtual void _init() {
		/*
		this->_map = BFMapCreate();
		if (!this->_map) return;
		BFMapSetCompare(this->_map, this->_BFMapCompare);
		BFMapSetRelease(this->_map, this->_BFMapRelease);
		*/
	}

	virtual void _deinit() {
		//BFMapRelease(this->_map);
	}

	size_t size() const {
		return BFMapGetSize(this->_map);
	}

	//int insert(K k, V v) {
	virtual int _insert(void * key, void * value) {
		if (!this->_map) return -1;
		return BFMapInsert(this->_map, key, value);
	}

	//V getValueForKey(K k, int * error) {
	virtual void * _getValueForKey(void * key, int * error) {
		if (!this->_map) return NULL;
		return BFMapGetValue(this->_map, key, error);
	}

	//int remove(K k) {
	virtual int _remove(void * key) {
		if (!this->_map) return -1;
		return BFMapRemove(this->_map, key);
	}

	//bool contains(K k) {
	virtual bool _contains(void * key) {
		if (!this->_map) return false;
		return BFMapContains(this->_map, key);
	}


	BFMap _map;
};
}

#endif // MAP_HPP

