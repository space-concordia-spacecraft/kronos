//
// Created by Mazen on 04.04.21.
//

#ifndef KRONOS_HASHMAP_H
#define KRONOS_HASHMAP_H

#include "hashnode.h"
#include "keyhash.h"
#include <cstddef>


// Hash map class template
template <typename K, typename V, size_t tableSize, typename F = KeyHash<K, tableSize> >
class HashMap
{
public:
    HashMap() :
            table(),
            hashFunc()
    {
    }

    ~HashMap()
    {
        // destroy all buckets one by one
        for (size_t i = 0; i < tableSize; ++i) {
            HashNode<K, V> *entry = table[i];

            while (entry != NULL) {
                HashNode<K, V> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            table[i] = NULL;
        }
    }

    bool get(const K &key, V &value)
    {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL) {
            if (entry->getKey() == key) {
                value = entry->getValue();
                return true;
            }

            entry = entry->getNext();
        }

        return false;
    }

    void put(const K &key, const V &value)
    {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = NULL;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            entry = new HashNode<K, V>(key, value);

            if (prev == NULL) {
                // insert as first bucket
                table[hashValue] = entry;

            } else {
                prev->setNext(entry);
            }

        } else {
            // just update the value
            entry->setValue(value);
        }
    }

    void remove(const K &key)
    {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = NULL;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            // key not found
            return;

        } else {
            if (prev == NULL) {
                // remove first bucket of the list
                table[hashValue] = entry->getNext();

            } else {
                prev->setNext(entry->getNext());
            }

            delete entry;
        }
    }

private:
    HashMap(const HashMap & other);
    const HashMap & operator=(const HashMap & other);
    // hash table
    HashNode<K, V> *table[tableSize];
    F hashFunc;
};

#endif //KRONOS_HASHMAP_H
