#include "ks_queue.h"

class QueueFixture : public ::testing::Test {

protected:
    kronos::Queue<int> queue;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


TEST_F(QueueFixture, QueuePushTest){
    queue.clear();
    queue.push(10);
    queue.push(11);
    EXPECT_EQ(queue[0], 10);
    EXPECT_NE(queue[0], 11);
    EXPECT_EQ(queue[1], 11);
}

TEST_F(QueueFixture, QueuePopTest){
    queue.clear();
    queue.push(10);
    queue.push(11);
    queue.pop();
    EXPECT_EQ(queue[0], 11);
    EXPECT_NE(queue[0], 10);
    EXPECT_EQ(queue.size(), 1);
}

TEST_F(QueueFixture, QueuePeekTest){
    queue.clear();
    queue.push(10);
    queue.push(11);
    EXPECT_EQ(queue.peek(), 10);
    EXPECT_EQ(queue.peek(), queue[0]);
    queue.pop()
    EXPECT_EQ(queue.peek(), 11);
    EXPECT_EQ(queue.peek(), queue[0]);
    EXPECT_EQ(queue.size(), 1);
}

TEST_F(QueueFixture, QueueClearTest){
    queue.clear();
    queue.push(10);
    queue.push(11);
    queue.clear();
    EXPECT_EQ(vector.size(), 0);
    queue.push(12);
    EXPECT_EQ(vector[0], 12);
}

TEST_F(QueueFixture, QueueCapacityTest){
    queue.clear();
    queue.push(10);
    EXPECT_EQ(queue.capacity(), 10);
}