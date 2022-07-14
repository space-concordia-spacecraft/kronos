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
    struct has_hash : std::false_type {
    };

    template<class T>
    struct has_hash<T, decltype((void) &T::Hash, void())> : std::true_type {
    };

    template<int T, typename K>
    struct KeyHash {
    };

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

    //! \class HashNode class template
    //!
    //! \tparam K is the Key Type
    //! \tparam V is the Value Type
    template<typename K, typename V>
    class HashNode {
    public:
        //! \brief HashNode Contstructor
        //!
        //! \param key for the HashNode
        //! \param value for the HashNode
        HashNode(const K& key, const V& value) : m_Key(key), m_Value(value), m_Next(nullptr) {}

        //! \brief Getter for HashNode Key
        //! \return Key for the HashNode
        K& GetKey() { return m_Key; }

        //! \brief Getter for the HashNode Value
        //! \return Value for the HashNode
        V& GetValue() { return m_Value; }

        //! \brief Getter for Next HashNode
        //!
        //! \return Address to the next HashNode
        HashNode* GetNext() const { return m_Next; }

        //! \brief Setter for the HashNode Value
        //!
        //! \param value for the HasNode
        void SetValue(V value) { m_Value = value; }

        void SetNext(HashNode* next) { m_Next = next; }

    private:
        // Key-value pair
        //! Key of the HashNode
        K m_Key;
        //! Value of the HashNode
        V m_Value;

        // Next bucket with the same key
        //! Pointer to the next HashNode
        HashNode* m_Next;
    };

    template<typename K, typename V, typename F>
    class HashMap;

    //! \class HashMapIterator Class
    //! \tparam K is the Key of the HashNode
    //! \tparam V is the Value of the HashNode
    //! \tparam F is the Hashing Function
    template<typename K, typename V, typename F>
    class HashMapIterator {
    public:
        //! \brief HashMapIterator Parametrized Constructor
        //!
        //! \param hashMap for HashMapIterator
        //! \param hashIndex for HashMapIterator
        //! \param keyIndex for HashMapIterator
        HashMapIterator(const HashMap<K, V, F>* hashMap, size_t hashIndex, size_t keyIndex)
                : m_HashMap(hashMap), m_HashIndex(hashIndex), m_KeyIndex(keyIndex) {}

        //! \brief HashMapIterator ++ Unary Operator Overload
        HashMapIterator<K, V, F>& operator++() {
            // Checks if the HashMap is Hashable
            if (m_HashIndex == KS_HASHTABLE_SIZE - 1 && m_KeyIndex == KS_HASH_INVALID_KEY_INDEX)
                return *this;
            // Creates a HashNode with the Key,Value Pair
            HashNode<K, V>* node = &(**this);
            // Checks if the HashNode is not NULL and if it is connected to another nodes.
            if (node != nullptr && node->GetNext() != nullptr) {
                // Increments the Key Index
                m_KeyIndex++;
                return *this;
            }
            // Iterates over the HashMap
            for (size_t hashIndex = m_HashIndex + 1; hashIndex < KS_HASHTABLE_SIZE; hashIndex++) {
                // Checks if the Bucket is NULL
                if (m_HashMap->m_HashTable[hashIndex] != nullptr) {
                    m_HashIndex = hashIndex;
                    m_KeyIndex = 0;
                    return *this;
                }
            }

            m_HashIndex = KS_HASHTABLE_SIZE - 1;
            node = m_HashMap->m_HashTable[m_HashIndex];
            m_KeyIndex = KS_HASH_INVALID_KEY_INDEX;
            // Iterates through the bucket
            while (node != nullptr && node->GetNext() != nullptr) {
                node = node->GetNext();
                m_KeyIndex++;
            }

            return *this;
        }

        //! \brief HaspMapIterator ++ Binary Operator Overload
        HashMapIterator<K, V, F> operator++(int) {
            //Stores the Pointer Address of the HashMapIterator
            HashMapIterator temp = *this;
            //Increments the Address of the HashMap
            ++*this;
            //Returns the Original Address
            return temp;
        }

        //! HashMapIterator Dereference Operator Overload
        HashNode<K, V>& operator*() {
            // Creates a HashNode from the HashMap
            HashNode<K, V>* node = m_HashMap->m_HashTable[m_HashIndex];
            //Iterates through the HashNodes List
            for (size_t i = 0; i < m_KeyIndex; i++)
                node = node->GetNext();
            //Returns the Last Node
            return *node;
        }

        //! HashMapIterator Reference (->) Operator Overload
        HashNode<K, V>* operator->() {
            return &(**this);
        }

        //! \brief Equals Operator for the HashMapIterator
        //! \tparam K_ is the Key
        //! \tparam V_ is the Value
        //! \tparam F_ is the Hashing Function
        //! \param left is the first HashMapIterator
        //! \param right is the second HashMapIterator
        //! \return Boolean Expression showing if two HashMaps are equal
        template<typename K_, typename V_, typename F_>
        friend bool operator==(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right);

        //!
        //! \tparam K_ is the Key
        //! \tparam V_ is the Value
        //! \tparam F_ is the Hashing Function
        //! \param left is the first HashMapIterator
        //! \param right is the second HashMapIterator
        //! \return Boolean Expression showing if two HashMaps are not equal
        template<typename K_, typename V_, typename F_>
        friend bool operator!=(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right);

    private:
        //! The HashMap that will be iterated.
        const HashMap<K, V, F>* m_HashMap;
        //! Hash Index
        size_t m_HashIndex;
        //! Bucket Index
        size_t m_KeyIndex;
    };

    //! \tparam K is the Key
    //! \tparam V is the Value
    //! \tparam F is the Hashing Function
    //! \brief Hash map class template
    template<typename K, typename V, typename F = KeyHash<
            std::is_class<K>::value && has_hash<K>::value ? KS_HASH_MEMBER_FN :
            std::is_class<K>::value ? KS_HASH_CLASS : KS_HASH_TYPE, K>>
    class HashMap : public Iterable<HashMapIterator<K, V, F>> {
        friend class HashMapIterator<K, V, F>;

    public:
        //! \brief HashMap  Default constructor
        HashMap() : m_HashTable(), m_HashFunction() {}

        //! \brief HashMap Destructor
        ~HashMap() {
            // Destroys all buckets one after another
            for (auto& i: m_HashTable) {
                HashNode<K, V>* entry = i;

                while (entry != nullptr) {
                    HashNode<K, V>* prev = entry;
                    entry = entry->GetNext();
                    delete prev;
                }

                i = nullptr;
            }
        }

        //! \brief Getter for the HashMap Value
        //! \param key is the bucket key
        //! \return HashNode
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

        //! \brief Function to check if the Key,Value pair exists
        //!
        //! \param key the key for the HashNode
        //! \param value the value for the HashNode
        //! \return Logical Expression if the Key,Value pair exists.
        bool Peek(const K& key, V* value) {
            //Stores the Hash value from the Hashing Function
            unsigned long hashValue = m_HashFunction(key);
            //Retrieves the HashNode with the hash value.
            HashNode<K, V>* entry = m_HashTable[hashValue];
            //Iterates through the list to find the Node with the corresponding Key
            while (entry != nullptr) {
                // Checks if the Node has the desired Key
                if (entry->GetKey() == key) {
                    *value = entry->GetValue();
                    //Node found
                    return true;
                }

                entry = entry->GetNext();
            }
            //Returns False since the Node does not exist
            return false;
        }

        //! \brief Function to Insert a <Key,Value> pair in the HashMap
        //! \param key is the Key that is to be inserted
        //! \param value is the Value that is to be inserted
        void Put(const K& key, const V& value) {
            // Finds the Hash Value for the Key
            unsigned long hashValue = m_HashFunction(key);
            // Previous Reference is initialized to Nullptr
            HashNode<K, V>* prev = nullptr;
            //Retrieves the HashNode with the HashValue
            HashNode<K, V>* entry = m_HashTable[hashValue];
            //Iterates until the Key is found.
            while (entry != nullptr && entry->GetKey() != key) {
                prev = entry;
                entry = entry->GetNext();
            }
            // Checks if the HashNode is NULL
            if (entry == nullptr) {
                // Creates a new HashNode with the <Key,Value> Pair
                entry = new HashNode<K, V>(key, value);
                //Sets the previous reference to the last node.
                if (prev == nullptr) {
                    // insert as first bucket
                    m_HashTable[hashValue] = entry;
                    // Adds the new HashNode Reference to the List
                } else {
                    prev->SetNext(entry);
                }

            } else {
                // Updates the Value
                entry->SetValue(value);
            }
        }

        //! \brief Function that removes the HashNode with the Key
        //! \param key is the key of the HashNode
        void Remove(const K& key) {
            // Hash value of the node.
            unsigned long hashValue = m_HashFunction(key);
            // Previous Reference is initialized to Nullptr
            HashNode<K, V>* prev = nullptr;
            // Finds the Node with the Hash Value.
            HashNode<K, V>* entry = m_HashTable[hashValue];
            //Iterates through the HashNode List
            while (entry != nullptr && entry->GetKey() != key) {
                prev = entry;
                entry = entry->GetNext();
            }
            //Checks if the Node exists
            if (entry == nullptr) {
                // Key was not found in the List
                return;

            } else {
                if (prev == nullptr) {
                    // Removes the first bucket from the list
                    m_HashTable[hashValue] = entry->GetNext();

                } else {
                    //Links the previous and next nodes
                    prev->SetNext(entry->GetNext());
                }
                // Deletes the Node
                delete entry;
            }
        }

        //! \brief Function which returns the size of the HashMap
        size_t Size() { return 0; } //TODO
        //! Function to access the buckets of the HashMap
        //! \param key is the key of the HashNode
        //! \return Bucket from the HashMap
        V& operator[](const K& key) { return Get(key); }

        //! \brief Function which starts the HashMapIterator from beginning
        //! \return beginning of the HashMap
        HashMapIterator<K, V, F> begin() const override { //TODO test iteration
            //Iterates through the Hash Map
            for (size_t i = 0; i < KS_HASHTABLE_SIZE; i++) {
                // Checks if the bucket is not empty
                if (m_HashTable[i] != nullptr) {
                    return HashMapIterator<K, V, F>(this, i, 0);
                }
            }
            // Returns the beginning bucket of the HashMap
            return HashMapIterator<K, V, F>(this, KS_HASHTABLE_SIZE - 1, 0);
        }

        //! \brief Function which returns the HashMapIterator from the end
        //! \return end of the HashMap
        HashMapIterator<K, V, F> end() const override {
            // Finds the beginning of the HashMap
            auto start = begin();
            //End is initialized as the Start of the HashMap
            auto last = start;
            //Increment the Start
            start++;
            // Iterates until the end of the bucket
            while (start != last) {
                last = start;
                start++;
            }
            return start;
        }

    private:
        //! HashTable made up of HashNodes
        HashNode<K, V>* m_HashTable[KS_HASHTABLE_SIZE];
        //! Hashing Function
        F m_HashFunction;
    };

    //!
    //! \tparam K_ is the Key
    //! \tparam V_ is the Value
    //! \tparam F_ is the Hashing Function
    //! \param left is the first HashMapIterator to be compared
    //! \param right is the second HashMapIterator to be compared
    //! \return  Logical Expression if two HashMaps are the same
    template<typename K_, typename V_, typename F_>
    bool operator==(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right) {
        return left.m_HashMap == right.m_HashMap &&
               left.m_HashIndex == right.m_HashIndex &&
               left.m_KeyIndex == right.m_KeyIndex;
    }

    //!
    //! \tparam K_ is the Key
    //! \tparam V_ is the Value
    //! \tparam F_ is the Hashing Function
    //! \param left is the first HashMapIterator to be compared
    //! \param right is the second HashMapIterator to be compared
    //! \return  Logical Expression if two HashMaps are not the same
    template<typename K_, typename V_, typename F_>
    bool operator!=(const HashMapIterator<K_, V_, F_>& left, const HashMapIterator<K_, V_, F_>& right) {
        return !(left == right);
    }

}
