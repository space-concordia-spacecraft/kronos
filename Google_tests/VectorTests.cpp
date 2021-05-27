#include "gtest/gtest.h"
#include "ks_vector.h"

class VectorFixture : public ::testing::Test {

protected:
    kronos::Vector<int>* vector;

    virtual void SetUp() {
        vector = new kronos::Vector<int>();
    }

    virtual void TearDown() {
        delete vector;
    }

};


TEST_F(VectorFixture, PutAndGetSuccessScenario){
    int val;
    vector->add(10);

    EXPECT_TRUE(vector->contains(10));
    EXPECT_EQ(val, 10);

//    EXPECT_TRUE(vector->remove(10));
}

TEST_F(VectorFixture, PutAndGetFailingScenario){
//    EXPECT_FALSE(vector->contains(10));
}