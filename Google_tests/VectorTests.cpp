#include "gtest/gtest.h"
#include "ks_vector.h"

class VectorFixture : public ::testing::Test {

protected:
    kronos::Vector<int> vec;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

};


TEST_F(VectorFixture, PutAndGetSuccessScenario){
    int val;
    vec.add(8);

    EXPECT_TRUE(vec[0]);
    EXPECT_EQ(val, 8);
}

TEST_F(VectorFixture, PutAndGetFailingScenario){
    EXPECT_FALSE(map->get(0));
}