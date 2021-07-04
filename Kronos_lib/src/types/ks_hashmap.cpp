#include "ks_hashmap.h"

namespace kronos {

    // ================== Default Hash Function ==================

    template<typename K>
    uint32_t KeyHash<K>::operator()(const K& key) const {
        return key & (KS_HASHTABLE_SIZE - 1);
    }


    // ================== Hash Node ==================

    template<typename K, typename V>
    HashNode<K, V>::HashNode(const K& key, const V& value)
        : m_Key(key), m_Value(value), m_Next(nullptr) {}

    template<typename K, typename V>
    K & HashNode<K, V>::GetKey() {
        return m_Key;
    }

    template<typename K, typename V>
    V & HashNode<K, V>::GetValue() {
        return m_Value;
    }

    template<typename K, typename V>
    HashNode<K, V> * HashNode<K, V>::GetNext() const {
        return m_Value;
    }

    template<typename K, typename V>
    void HashNode<K, V>::SetValue(V value) {
        m_Value = value;
    }

    template<typename K, typename V>
    void HashNode<K, V>::SetNext(HashNode * next) {
        m_Next = next;
    }


    // ================== Hash Map ==================

    template<typename K, typename V, typename F>
    HashMap<K, V, F>::HashMap()
            : m_HashTable(), m_HashFunction() {}

    template<typename K, typename V, typename F>
    HashMap<K, V, F>::~HashMap() {
        // destroy all buckets one by one
        for (auto & i : m_HashTable) {
            HashNode<K, V> * entry = i;

            while (entry != nullptr) {
                HashNode<K, V> * prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            i = nullptr;
        }
    }

    template<typename K, typename V, typename F>
    V & HashMap<K, V, F>::Get(const K & key) {
        unsigned long hashValue = m_HashFunction(key);
        HashNode<K, V> * entry = m_HashTable[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == key) {
                return entry->getValue();
            }

            entry = entry->getNext();
        }

        Put(key, V());
        return Get(key);
    }

    template<typename K, typename V, typename F>
    bool HashMap<K, V, F>::Peek(const K & key, V * value) {
        unsigned long hashValue = m_HashFunction(key);
        HashNode<K, V> * entry = m_HashTable[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == key) {
                * value = entry->getValue();
                return true;
            }

            entry = entry->getNext();
        }

        return false;
    }

    template<typename K, typename V, typename F>
    void HashMap<K, V, F>::Put(const K & key, const V & value) {
        unsigned long hashValue = m_HashFunction(key);
        HashNode<K, V> * prev = nullptr;
        HashNode<K, V> * entry = m_HashTable[hashValue];

        while (entry != nullptr && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            entry = new HashNode<K, V>(key, value);

            if (prev == nullptr) {
                // insert as first bucket
                m_HashTable[hashValue] = entry;

            } else {
                prev->setNext(entry);
            }

        } else {
            // just update the value
            entry->setValue(value);
        }
    }

    template<typename K, typename V, typename F>
    void HashMap<K, V, F>::Remove(const K & key) {
        unsigned long hashValue = m_HashFunction(key);
        HashNode<K, V> * prev = nullptr;
        HashNode<K, V> * entry = m_HashTable[hashValue];

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
                m_HashTable[hashValue] = entry->getNext();

            } else {
                prev->setNext(entry->getNext());
            }

            delete entry;
        }
    }

    template<typename K, typename V, typename F>
    V & HashMap<K, V, F>::operator[](const K & key) {
        return Get(key);
    }

}
