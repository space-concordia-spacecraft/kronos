#include "KronosTest.h"
#include "ks_hashmap.h"

KT_TEST(MapPut) {
    kronos::HashMap<int, int> map;

    map.Put(0, 0);
    map.Put(1, 10);
    map.Put(2, 20);
    map.Put(3, 30);

    KT_ASSERT(map[0] == 0);
    KT_ASSERT(map[1] == 10);
    KT_ASSERT(map[2] == 20);
    KT_ASSERT(map[3] == 30);

    return true;
}

KT_TEST(MapRemove) {
    kronos::HashMap<int, int> map;

    int x = 0;

    map.Put(0, 0);
    map.Put(1, 10);
    map.Put(2, 20);

    KT_ASSERT(map.Peek(0, &x));
    KT_ASSERT(x == 0);

    KT_ASSERT(map.Peek(1, &x));
    KT_ASSERT(x == 10);

    KT_ASSERT(map.Peek(2, &x));
    KT_ASSERT(x == 20);

    map.Remove(0);
    map.Remove(2);

    KT_ASSERT(!map.Peek(0, &x));

    KT_ASSERT(map.Peek(1, &x));
    KT_ASSERT(x == 10);

    KT_ASSERT(!map.Peek(2, &x));

    return true;
}