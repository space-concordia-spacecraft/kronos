#pragma once

#define KS_HASHTABLE_SIZE           16
#define KS_HASH_MEMBER_FN           0
#define KS_HASH_CLASS               1
#define KS_HASH_TYPE                2
#define KS_HASH_INVALID_KEY_INDEX   0xFFFFFFFF

#include "asf.h"
#include <type_traits>

#include "ks_iterable.h"

namespace kronos {

    template<typename T, typename = void>
    struct has_hash : std::false_type {};

    template<class T>
    struct has_hash<T, decltype((void) &T::Hash, void())> : std::true_type {};

    template<int T, typename K>
    struct KeyHash {};

    // Default hash function class
    template<typename K>
    struct KeyHash<KS_HASH_MEMBER_FN, K> {
        uint32_t operator()(const K& key) const {
            return key.Hash() & static_cast<uint32_t>(KS_HASHTABLE_SIZE - 1);
        }
    };

    template<typename K>
    struct KeyHash<KS_HASH_CLASS, K> {
        uint32_t operator()(const K& key) const {
            return (uint32_t) (&key) & static_cast<uint32_t>(KS_HASHTABLE_SIZE - 1);
        }
    };

    template<typename K>
    struct KeyHash<KS_HASH_TYPE, K> {
        uint32_t operator()(const K& key) const {
            return (uint32_t) (key) & static_cast<uint32_t>(KS_HASHTABLE_SIZE - 1);
        }
    };

    // Hash node class template
    template<typename K, typename V>
    class HashNode {
    public:
        HashNode(const K& key, const V& value) : m_Key(key), m_Value(value), m_Next(nullptr) {}

        K& GetKey() { return m_Key; }
        V& GetValue() { return m_Value; }
        HashNode* GetNext() const { return m_Next; }

        void SetValue(V value) { m_Value = value; }
        void SetNext(HashNode* next) { m_Next = next; }

    private:
        // Key-value pair
        K m_Key;
        V m_Value;

        // Next bucket with the same key
        HashNode* m_Next;
    };

    template<typename K, typename V, typename F>
    class HashMap;

    template<typename K, typename V, typename F>
    class HashMapIterator {
    public:
        HashMapIterator(const HashMap<K, V, F>* hashMap, size_t hashIndex, size_t keyIndex)
                : m_HashMap(hashMap), m_HashIndex(hashIndex), m_KeyIndex(keyIndex) {}

        HashMapIterator<K, V, F>& operator++() {
            if (m_HashIndex == KS_HASHTABLE_SIZE - 1 && m_KeyIndex == KS_HASH_INVALID_KEY_INDEX)
                return *this;

            HashNode<K, V>* node = &(**this);
            if (node != nullptr && node->GetNext() != nullptr) {
                m_KeyIndex++;
                return *this;
            }

            for (size_t hashIndex = m_HashIndex + 1; hashIndex < KS_HASHTABLE_SIZE; hashIndex++) {
                if (m_HashMap->m_HashTable[hashIndex] != nullptr) {
                    m_HashIndex = hashIndex;
                    m_KeyIndex = 0;
                    return *this;
                }
            }

            m_HashIndex = KS_HASHTABLE_SIZE - 1;
            node = m_HashMap->m_HashTable[m_HashIndex];
            m_KeyIndex = KS_HASH_INVALID_KEY_INDEX;
            while (node != nullptr && node->GetNext() != nullptr) {
                node = node->GetNext();
                m_KeyIndex++;
            }

            return *this;
        }

        HashMapIterator<K, V, F> operator++(int) {
            HashMapIterator temp = *this;
            ++*this;
            return temp;
        }

        HashNode<K, V>& operator*() {
            HashNode<K, V>* node = m_HashMap->m_HashTable[m_HashIndex];
            for (size_t i = 0; i < m_KeyIndex; i++)
                node = node->GetNext();
            return *node;
        }

        HashNode<K, V>* operator->() {
            return &(**this);
        }

        template<typename K_, typename V_, typename F_>
        friend bool operator==(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right);

        template<typename K_, typename V_, typename F_>
        friend bool operator!=(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right);

    private:
        const HashMap<K, V, F>* m_HashMap;
        size_t m_HashIndex;
        size_t m_KeyIndex;
    };

    // Hash map class template
    template<typename K, typename V, typename F = KeyHash<
            std::is_class<K>::value && has_hash<K>::value ? KS_HASH_MEMBER_FN :
            std::is_class<K>::value ? KS_HASH_CLASS : KS_HASH_TYPE, K>>
    class HashMap : public Iterable<HashMapIterator<K, V, F>> {
        friend class HashMapIterator<K, V, F>;

    public:
        HashMap() : m_HashTable(), m_HashFunction() {}

        ~HashMap() {
            // destroy all buckets one by one
            for (auto& i : m_HashTable) {
                HashNode<K, V>* entry = i;

                while (entry != nullptr) {
                    HashNode<K, V>* prev = entry;
                    entry = entry->GetNext();
                    delete prev;
                }

                i = nullptr;
            }
        }

        V& Get(const K& key) {
            unsigned long hashValue = m_HashFunction(key);
            HashNode<K, V>* entry = m_HashTable[hashValue];

            while (entry != nullptr) {
                if (entry->GetKey() == key) {
                    return entry->GetValue();
                }

                entry = entry->GetNext();
            }

            Put(key, V());
            return Get(key);
        }

        bool Peek(const K& key, V* value) {
            unsigned long hashValue = m_HashFunction(key);
            HashNode<K, V>* entry = m_HashTable[hashValue];

            while (entry != nullptr) {
                if (entry->GetKey() == key) {
                    *value = entry->GetValue();
                    return true;
                }

                entry = entry->GetNext();
            }

            return false;
        }

        void Put(const K& key, const V& value) {
            unsigned long hashValue = m_HashFunction(key);
            HashNode<K, V>* prev = nullptr;
            HashNode<K, V>* entry = m_HashTable[hashValue];

            while (entry != nullptr && entry->GetKey() != key) {
                prev = entry;
                entry = entry->GetNext();
            }

            if (entry == nullptr) {
                entry = new HashNode<K, V>(key, value);

                if (prev == nullptr) {
                    // insert as first bucket
                    m_HashTable[hashValue] = entry;

                } else {
                    prev->SetNext(entry);
                }

            } else {
                // just update the value
                entry->SetValue(value);
            }
        }

        void Remove(const K& key) {
            unsigned long hashValue = m_HashFunction(key);
            HashNode<K, V>* prev = nullptr;
            HashNode<K, V>* entry = m_HashTable[hashValue];

            while (entry != nullptr && entry->GetKey() != key) {
                prev = entry;
                entry = entry->GetNext();
            }

            if (entry == nullptr) {
                // key not found
                return;

            } else {
                if (prev == nullptr) {
                    // remove first bucket of the list
                    m_HashTable[hashValue] = entry->GetNext();

                } else {
                    prev->SetNext(entry->GetNext());
                }

                delete entry;
            }
        }

        size_t Size() { return 0; } //TODO

        V& operator[](const K& key) { return Get(key); }

        HashMapIterator<K, V, F> begin() const override { //TODO test iteration
            for (size_t i = 0; i < KS_HASHTABLE_SIZE; i++) {
                if (m_HashTable[i] != nullptr) {
                    return HashMapIterator<K, V, F>(this, i, 0);
                }
            }
            return HashMapIterator<K, V, F>(this, KS_HASHTABLE_SIZE - 1, 0);
        }

        HashMapIterator<K, V, F> end() const override {
            auto start = begin();
            auto last = start;
            start++;
            while (start != last) {
                last = start;
                start++;
            }
            return start;
        }

    private:
        HashNode<K, V>* m_HashTable[KS_HASHTABLE_SIZE];
        F m_HashFunction;
    };

    template<typename K_, typename V_, typename F_>
    bool operator==(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right) {
        return  left.m_HashMap == right.m_HashMap &&
                left.m_HashIndex == right.m_HashIndex &&
                left.m_KeyIndex == right.m_KeyIndex;
    }

    template<typename K_, typename V_, typename F_>
    bool operator!=(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right) {
        return !(left == right);
    }

}
