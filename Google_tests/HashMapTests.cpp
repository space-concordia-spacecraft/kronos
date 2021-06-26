#include "gtest/gtest.h"
#include "ks_hashmap.h"

class HashMapFixture : public ::testing::Test {

protected:
    kronos::HashMap<int, int> map;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

};


TEST_F(HashMapFixture, PutAndGetTest) {
    map.put(0, 10);
    map.put(16, 70);
    map.put(1, 20);
    map.put(2, 13);
    map.put(5, 45);

    EXPECT_EQ(map.get(16), 70);
    EXPECT_EQ(map.get(0), 10);
    EXPECT_EQ(map.get(5), 45);
    EXPECT_EQ(map.get(2), 13);
    EXPECT_EQ(map.get(1), 20);


}

TEST_F(HashMapFixture, RemoveTest) {
    map.put(0, 10);
    map.put(16, 70);
    map.put(1, 20);
    map.put(2, 13);
    map.put(5, 45);

    EXPECT_EQ(map.get(16), 70);
    EXPECT_EQ(map.get(0), 10);
    EXPECT_EQ(map.get(5), 45);
    EXPECT_EQ(map.get(2), 13);
    EXPECT_EQ(map.get(1), 20);

    map.remove(0);
    map.remove(16);
    map.remove(1);

    EXPECT_NE(map.get(16), 70);
    EXPECT_NE(map.get(0), 10);
    EXPECT_EQ(map.get(5), 45);
    EXPECT_EQ(map.get(2), 13);
    EXPECT_NE(map.get(1), 20);
}