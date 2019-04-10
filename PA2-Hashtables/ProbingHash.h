#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>
#include "Hash.h"

using std::vector;
using std::pair;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> {
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion
    vector<pair<pair<K,V>, EntryState>> _table;
    int _size;

public:
    ProbingHash(int n = 11) {
        _size = 0;
        pair<pair<K,V>, EntryState> p;
        p.second = EntryState::EMPTY;
        _table.resize(n, p);
    }

    ~ProbingHash() {
        // Needs to actually free all table contents
        _table.clear();
        _size = 0;
    }

    bool empty() {
        return !_size;
    }

    int size() {
        return _size;
    }

    V& at(const K& key) {
        if (_table[hash(key)].second != EntryState::VALID) {
            throw std::out_of_range("Key not in hash");
        }
        return _table[hash(key)].first.second;
    }

    V& operator[](const K& key) {
        return this->at(key);
    }

    int count(const K& key) {
        
    }

    void emplace(K key, V value) {
        pair<K,V> p(key, value);
        insert(p);
    }

    void insert(const std::pair<K, V>& pair) {
        if (load_factor() > 0.5){
            rehash();
        }
        _table[hash(pair.first)].first = pair;
        _table[hash(pair.first)].second = EntryState::VALID;
        ++_size;
    }

    void erase(const K& key) {
        if (_table[hash(key)].second != EntryState::EMPTY) {
            _table[hash(key)].second = EntryState::DELETED;
            _size--;
        }
    }

    void clear() {
        for (int i = 0; i < _table.capacity(); i++){
            this->erase(i);
        }
        _size = 0;
        //_table.clear();
    }

    int bucket_count() {
        return _table.capacity();
    }

    int bucket_size(int n) {
        
    }

    int bucket(const K& key) {
    }

    float load_factor() {
        return (float)_size/(float)_table.capacity();
    }

    void rehash() {
        _table.resize(findNextPrime(_table.capacity()));
    }

    void rehash(int n) {
        _table.resize(findNextPrime(n));
    }

private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key) {
        int hashKey = key % _table.capacity();
        if(_table[hashKey].second != EntryState::EMPTY){
            return probe(hashKey);
        }
        else {
            return hashKey;
        }
    }
    int probe(int i){
        return ((i+1) % _table.capacity());
    }
    
};

#endif //__PROBING_HASH_H
