#include <string.h>
#include "hashmap.h"



struct MyKeyHash {
    unsigned long operator()(const int& k) const
    {
        return k % 10;
    }
};

int main() {
    size_t tableSize = 10;
    HashMap<int, char*, 10, MyKeyHash> hmap;

    char val1[10] = "val1";
    char val2[10] = "val2";
    char val3[10] = "val3";

    hmap.put(1, val1);
    hmap.put(2, val2);
    hmap.put(3, val3);

    char *value;
    hmap.get(2, value);

    bool res = hmap.get(3, value);

    hmap.remove(3);
    res = hmap.get(3, value);

    return 0;
}