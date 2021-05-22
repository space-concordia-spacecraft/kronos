#pragma once

#define KS_HASHTABLE_SIZE 16

// Default hash function class
template<typename K>
struct KeyHash {
    uint32_t operator()(const K & key) const {
        return key & (KS_HASHTABLE_SIZE - 1);
    }
};

// Hash node class template
template<typename K, typename V>
class HashNode {
public:
    HashNode(const K & key, const V & value) :
            _key(key), _value(value), _next(NULL) {}

    K & getKey() {
        return _key;
    }

    V & getValue() {
        return _value;
    }

    void setValue(V value) {
        _value = value;
    }

    HashNode * getNext() const {
        return _next;
    }

    void setNext(HashNode * next) {
        _next = next;
    }

private:
    // key-value pair
    K _key;
    V _value;
    // next bucket with the same key
    HashNode * _next;
};

// Hash map class template
template<typename K, typename V, typename F = KeyHash<K>>
class HashMap {
public:
    HashMap() :
            table(),
            hashFunc() {}

    ~HashMap() {
        // destroy all buckets one by one
        for (auto & i : table) {
            HashNode<K, V> * entry = i;

            while (entry != nullptr) {
                HashNode<K, V> * prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            i = nullptr;
        }
    }

    V & get(const K & key) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> * entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == key) {
                return entry->getValue();
            }

            entry = entry->getNext();
        }

        put(key, V());
        return get(key);
    }

    void put(const K & key, const V & value) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> * prev = nullptr;
        HashNode<K, V> * entry = table[hashValue];

        while (entry != nullptr && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            entry = new HashNode<K, V>(key, value);

            if (prev == nullptr) {
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

    void remove(const K & key) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> * prev = nullptr;
        HashNode<K, V> * entry = table[hashValue];

        while (entry != nullptr && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            // key not found
            return;

        } else {
            if (prev == nullptr) {
                // remove first bucket of the list
                table[hashValue] = entry->getNext();

            } else {
                prev->setNext(entry->getNext());
            }

            delete entry;
        }
    }

    V & operator[](const K & key) {
        return get(key);
    }

private:
    // hash table
    HashNode<K, V> * table[KS_HASHTABLE_SIZE];
    F hashFunc;
};
