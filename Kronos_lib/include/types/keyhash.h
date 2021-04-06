//
// Created by Mazen on 04.04.21.
//

#ifndef KRONOS_KEYHASH_H
#define KRONOS_KEYHASH_H


// Default hash function class
template <typename K, size_t tableSize>
struct KeyHash {
    unsigned long operator()(const K &key) const {
        return reinterpret_cast<unsigned long>(key) % tableSize;
    }
};
#endif //KRONOS_KEYHASH_H
