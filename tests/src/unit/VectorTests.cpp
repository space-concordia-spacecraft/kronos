#define CATCH_CONFIG_MAIN

#include "ks_vector.h"
#undef FAIL
#include "../../include/catch_amalgamated.hpp"

TEST_CASE("Add Method Test ", "[VectorAddTest]") {
    kronos::Vector<int> vector;
    vector.add(10);
    vector.add(11);
    CHECK(vector[0] == 10);
    CHECK(vector[0] != 11);
    CHECK(vector[1] == 11);
}

TEST_CASE("Contain Method Test ", "[VectorContainsTest]") {
    kronos::Vector<int> vector;
    vector.add(10);
    vector.add(11);
    CHECK(vector.contains(10) == 0);
    CHECK(vector.contains(12) == -1);
}

TEST_CASE("Remove Method Test ", "[VectorRemoveTest]") {
    kronos::Vector<int> vector;
    vector.add(10);
    vector.add(11);
    vector.remove(10);
    CHECK(vector.contains(10) == -1);
    CHECK(vector.contains(11) == 0);
    CHECK(vector[0] == 11);
    CHECK(vector.size() == 1);
}

TEST_CASE("Clear Method Test ", "[VectorClearTest]") {
    kronos::Vector<int> vector;
    vector.add(10);
    vector.add(11);
    vector.clear();
    CHECK(vector.size() == 0);
    vector.add(12);
    CHECK(vector[0] == 12);
}

TEST_CASE("Capacity Method Test ", "[VectorCapacityTest]") {
    kronos::Vector<int> vector;
    vector.add(10);
    CHECK(vector.capacity() == 10);
}