#include "ks_vector.h"

class VectorFixture : public ::testing::Test {

protected:
    kronos::Vector<int> vector;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

};


TEST_F(VectorFixture, VectorAddTest){
    vector.clear();
    vector.add(10);
    vector.add(11);
    EXPECT_EQ(vector[0], 10);
    EXPECT_NE(vector[0], 11);
    EXPECT_EQ(vector[1], 11);
}

TEST_F(VectorFixture, VectorContainsTest){
    vector.clear();
    vector.add(10);
    vector.add(11);
    EXPECT_EQ(vector.contains(10), 0);
    EXPECT_EQ(vector.contains(12), -1);
}

TEST_F(VectorFixture, VectorRemoveTest){
    vector.clear();
    vector.add(10);
    vector.add(11);
    vector.remove(10);
    EXPECT_EQ(vector.contains(10), -1);
    EXPECT_EQ(vector.contains(11), 0);
    EXPECT_EQ(vector[0], 11);
    EXPECT_EQ(vector.size(), 1);
}

TEST_F(VectorFixture, VectorClearTest){
    vector.clear();
    vector.add(10);
    vector.add(11);
    vector.clear();
    EXPECT_EQ(vector.size(), 0);
    vector.add(12);
    EXPECT_EQ(vector[0], 12);
}

TEST_F(VectorFixture, VectorCapacityTest){
    vector.clear();
    vector.add(10);
    EXPECT_EQ(vector.capacity(), 10);
}
