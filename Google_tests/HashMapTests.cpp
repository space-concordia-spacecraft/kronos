#include "gtest/gtest.h"
#include "ks_hashmap.h"

class HashMapFixture : public ::testing::Test {

protected:
    HashMap<int, int>* map;

    virtual void SetUp() {
        map = new HashMap<int, int>();
    }

    virtual void TearDown() {
        delete map;
    }

};


TEST_F(HashMapFixture, PutAndGetSuccessScenario){
    int val;
    map->put(0, 10);

    EXPECT_TRUE(map->get(0));
    EXPECT_EQ(val, 10);

    EXPECT_TRUE(map->get(0));
}

TEST_F(HashMapFixture, PutAndGetFailingScenario){
    EXPECT_FALSE(map->get(0));
}