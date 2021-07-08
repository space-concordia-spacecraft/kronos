#pragma once

#define KS_HASHTABLE_SIZE 16

#include "asf.h"

namespace kronos {

    // Default hash function class
    template<typename K>
    struct KeyHash {
        uint32_t operator()(const K& key) const;
    };

    // Hash node class template
    template<typename K, typename V>
    class HashNode {
    public:
        HashNode(const K& key, const V& value);

        K& GetKey();
        V& GetValue();
        HashNode* GetNext() const;

        void SetValue(V value);
        void SetNext(HashNode* next);

    private:
        // Key-value pair
        K m_Key;
        V m_Value;

        // Next bucket with the same key
        HashNode* m_Next;
    };

    // Hash map class template
    template<typename K, typename V, typename F = KeyHash<K>>
    class HashMap {
    public:
        HashMap();
        ~HashMap();

        V& Get(const K& key);
        bool Peek(const K& key, V* value);
        void Put(const K& key, const V& value);
        void Remove(const K& key);

        size_t Size();

        V& operator[](const K& key);

    private:
        HashNode<K, V>* m_HashTable[KS_HASHTABLE_SIZE];
        F m_HashFunction;
    };

}
