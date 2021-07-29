#define CATCH_CONFIG_MAIN
#include "../../include/catch_amalgamated.hpp"

#include "ks_hashmap.h"
#include "../../../lib/include/types/ks_hashmap.h"


TEST_CASE("Put And Get Test", "[MapPut]") {
    kronos::HashMap<int, int> map;

    map.Put(0, 10);
    map.Put(16, 70);
    map.Put(1, 20);
    map.Put(2, 13);
    map.Put(5, 45);

    CHECK(map.Get(16) == 70);
    CHECK(map.Get(0) == 10);
    CHECK(map.Get(5) == 45);
    CHECK(map.Get(2) == 13);
    CHECK(map.Get(1) == 20);

}

TEST_CASE("Remove Test", "[MapRemove]") {
    kronos::HashMap<int, int> map;

    map.Put(0, 10);
    map.Put(16, 70);
    map.Put(1, 20);
    map.Put(2, 13);
    map.Put(5, 45);

    CHECK(map.Get(16) == 70);
    CHECK(map.Get(0) == 10);
    CHECK(map.Get(5) == 45);
    CHECK(map.Get(2) == 13);
    CHECK(map.Get(1) == 20);

    map.Remove(0);
    map.Remove(16);
    map.Remove(1);

    CHECK(map.Get(16) != 70);
    CHECK(map.Get(0) != 10);
    CHECK(map.Get(1) != 20);

    CHECK(map.Get(5), 45);
    CHECK(map.Get(2), 13);
}