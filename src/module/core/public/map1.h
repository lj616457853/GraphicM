#pragma   once  
#ifndef MAP1_H
#define MAP1_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;
template<typename Key, typename Value>
class HashTable {
private:
	static const int upperTol = 10;
	static const int lowerTol = 2;
	static const int initCapacity = 7;
	int M;
	int size;
	map<Key, Value> * hashTable;

	int hash(Key key) {
		return (hashCode(key) & 0x7fffffff) % M; 
	}

	int hashCode(Key key) {
		std::hash<Key> key_hash;
		return key_hash(key);
	}
	void resize(int new_M)
	{
		map < Key, Value>* newhashTable = new map<Key, Value>[new_M];

		int oldM = M;
		this->M = new_M;
		for (int i = 0; i < oldM; i++)
		{
			map<Key, Value>& my_map = hashTable[i];
			for (auto [ind, val] : my_map)
			{
				newhashTable[hash(ind)].insert(make_pair(ind, val));
			}

		}
		delete[]hashTable;
		hashTable = nullptr;
		this->hashTable = newhashTable;
	}

public:

	HashTable(int M) {
		this->M = M;
		size = 0;
		hashTable = new map<Key, Value>[M];
	}

	HashTable() {
		this->M = initCapacity;
		new(this)HashTable(M); 
	}

	int getSize() {
		return size;
	}

	void add(Key key, Value value) {
		map<Key, Value>* my_map = &hashTable[hash(key)];
		if (my_map->count(key)) {
			my_map->find(key)->second = value;
		}
		else {
			my_map->insert(make_pair(key, value));
			size++;

			if (size >= upperTol * M)
				resize(2 * M);
		}
	}

	Value remove(Key key) {
		Value temp;
		map<Key, Value>* my_map = &hashTable[hash(key)];
		if (my_map->count(key))
		{
			temp = my_map->find(key)->second;
			my_map->erase(key);
			size--;

			if (size < lowerTol * M && M / 2 >= initCapacity)
				resize(M / 2);
		}
		return temp;

	}



	bool contains(Key key) {
		return hashTable[hash(key)].count(key);
	}

	Value* get(Key key) {
		return &(hashTable[hash(key)])[key]; 
	}

	void set(Key key, Value value) {
		map<Key, Value>& my_map = hashTable[hash(key)]; 
		if (!my_map.count(key)) {
			throw to_string(key) + " doesn't exist";
		}
		//assert(my_map.count(key)!=0);
		my_map.find(key)->second = value;
	}
};
#endif