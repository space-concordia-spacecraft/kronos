#include "gtest/gtest.h"


class KronosFixture : public ::testing::Test {

protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(KronosFixture, Test1){ // 1/1/1 -> 1/3/1

    EXPECT_EQ(1,1);

}