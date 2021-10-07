#include "KronosTest.h"
#include "ks_vector.h"

KT_TEST(VectorAddTest) {
    kronos::Vector<int> vector;
    vector.Add(10);
    vector.Add(11);
    KT_ASSERT(vector[0] == 10);
    KT_ASSERT(vector[0] != 11);
    KT_ASSERT(vector[1] == 11);
    return true;
}

KT_TEST(VectorContainsTest) {
    kronos::Vector<int> vector;
    vector.Add(10);
    vector.Add(11);
    KT_ASSERT(vector.Find(10) == 0);
    KT_ASSERT(vector.Find(12) == -1);
    return true;
}

KT_TEST(VectorRemoveTest) {
    kronos::Vector<int> vector;
    vector.Add(10);
    vector.Add(11);
    vector.Remove(10);
    KT_ASSERT(vector.Find(10) == -1);
    KT_ASSERT(vector.Find(11) == 0);
    KT_ASSERT(vector[0] == 11);
    KT_ASSERT(vector.Size() == 1);
    return true;
}

KT_TEST(VectorClearTest) {
    kronos::Vector<int> vector;
    vector.Add(10);
    vector.Add(11);
    vector.Clear();
    KT_ASSERT(vector.Size() == 0);
    vector.Add(12);
    KT_ASSERT(vector[0] == 12);
    return true;
}

KT_TEST(VectorCapacityTest) {
    kronos::Vector<int> vector;
    vector.Add(10);
    KT_ASSERT(vector.Capacity() == 10);
    return true;
}