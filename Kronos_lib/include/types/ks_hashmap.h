#pragma once

#define KS_HASHTABLE_SIZE 16

#include "asf.h"

namespace kronos {

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
                m_Key(key), m_Value(value), m_Next(nullptr) {}

        K & GetKey() {
            return m_Key;
        }

        V & GetValue() {
            return m_Value;
        }

        HashNode * GetNext() const {
            return m_Next;
        }

        void SetValue(V value) {
            m_Value = value;
        }

        void SetNext(HashNode * next) {
            m_Next = next;
        }

    private:
        // key-value pair
        K m_Key;
        V m_Value;
        // next bucket with the same key
        HashNode * m_Next;
    };

    // Hash map class template
    template<typename K, typename V, typename F = KeyHash<K>>
    class HashMap {
    public:
        HashMap()
            : m_HashTable(), m_HashFunction() {}

        ~HashMap() {
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

        V & Get(const K & key) {
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

        bool Peek(const K & key, V * value) {
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

        void Put(const K & key, const V & value) {
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

        void Remove(const K & key) {
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

        V & operator[](const K & key) {
            return Get(key);
        }

    private:
        // hash table
        HashNode<K, V> * m_HashTable[KS_HASHTABLE_SIZE];
        F m_HashFunction;
    };

}
