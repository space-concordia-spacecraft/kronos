//
// Created by Mazen on 06.04.21.
//

#include "gtest/gtest.h"
#include "hashmap.h"
#include "keyhash.h"


class HashMapFixture : public ::testing::Test {

protected:
    HashMap<int, int, 10, KeyHash<unsigned long, 10> >* map;

    virtual void SetUp() {
        map = new HashMap<int, int, 10, KeyHash<unsigned long, 10> >();
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
    int val;

    EXPECT_FALSE(map->get(0));
}