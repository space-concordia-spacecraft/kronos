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

KT_TEST(MapPeek) {
    kronos::HashMap<int, int> map;

    map.Put(0, 10);
    map.Put(16, 70);
    map.Put(1, 20);
    map.Put(2, 13);
    map.Put(5, 45);

    KT_ASSERT(map.Get(16) == 70);
    KT_ASSERT(map.Get(0) == 10);
    KT_ASSERT(map.Get(5) == 45);
    KT_ASSERT(map.Get(2) == 13);
    KT_ASSERT(map.Get(1) == 20);

    int value = 0;
    KT_ASSERT(map.Peek(0, &value));
    KT_ASSERT(value == 10);
    KT_ASSERT(map.Peek(16, &value));
    KT_ASSERT(value == 70);
    KT_ASSERT(map.Peek(1, &value));
    KT_ASSERT(value == 20);
    KT_ASSERT(map.Peek(2, &value));
    KT_ASSERT(value == 13);
    KT_ASSERT(map.Peek(5, &value));
    KT_ASSERT(value == 45);

    return true;
}