#include "gtest/gtest.h"
#include "ks_string.h"

class StringFixture : public ::testing::Test {

protected:
    kronos::String str;

    virtual void SetUp() {
        str = kronos::String("Kronos");
    }

    virtual void TearDown() {

    }

};


TEST_F(StringFixture, PutAndGetSuccessScenario){
int val;
map->put(0, 10);

EXPECT_TRUE(map->get(0));
EXPECT_EQ(val, 10);

EXPECT_TRUE(map->get(0));
}

TEST_F(StringFixture, PutAndGetFailingScenario){
EXPECT_FALSE(map->get(0));
}