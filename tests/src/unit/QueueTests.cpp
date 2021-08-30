#include "ks_queue.h"
#undef FAIL
#include "../../include/catch_amalgamated.hpp"


TEST_CASE("Pushing and Popping from the Queue", "[QueuePushAndPop]") {
    kronos::Queue<int> queue;
    queue.Clear();

    queue.Push(10);
    queue.Push(11);

    int *popped = nullptr;

    queue.Pop(popped);
    CHECK(*popped == 10);

    queue.Pop(popped);
    CHECK(*popped == 11);

    CHECK(queue.Size() == 1);
}


TEST_CASE("Testing queue peek", "QueuePeekTest"){
    kronos::Queue<int> queue;
    queue.Clear();

    queue.Push(10);
    queue.Push(11);

    int* peeked = nullptr;
    int* popped = nullptr;

    queue.Peek(peeked);
    CHECK(*peeked == 10);

    queue.Pop(popped);
    queue.Peek(peeked);

    CHECK(*peeked == 11);
    CHECK(queue.Size() == 1);
}

TEST_CASE("Testing clearing queue", "QueueClearTest") {
    kronos::Queue<int> queue;

    queue.Clear();
    queue.Push(10);
    queue.Push(11);
    queue.Clear();

    CHECK(queue.Size() == 0);

    queue.Push(12);
    int* popped = nullptr;
    queue.Pop(popped);
    CHECK(*popped == 12);
}

TEST_CASE("Testing queue capacity", "QueueCapacityTest") {
    kronos::Queue<int> queue;

    queue.Clear();
    queue.Push(10);

    CHECK(queue.Length() == 10);
}