#include "KronosTest.h"
#include "ks_queue.h"

KT_TEST(QueuePushAndPopTest) {
    kronos::Queue<int> queue;
    queue.Clear();

    queue.Push(10);
    queue.Push(11);
    queue.Push(12);

    int popped;

    KT_ASSERT(queue.Pop(&popped));
    KT_ASSERT(popped == 10);

    KT_ASSERT(queue.Pop(&popped));
    KT_ASSERT(popped == 11);

    KT_ASSERT(queue.Size() == 1);

    return true;
}

KT_TEST(QueuePeekTest) {
    kronos::Queue<int> queue;
    queue.Clear();

    queue.Push(10);
    queue.Push(11);

    int peeked = 0;
    int popped = 0;

    KT_ASSERT(queue.Peek(&peeked));
    KT_ASSERT(peeked == 10);

    KT_ASSERT(queue.Pop(&popped));
    KT_ASSERT(popped == 10);

    KT_ASSERT(queue.Peek(&peeked));
    KT_ASSERT(peeked == 11);

    KT_ASSERT(queue.Size() == 1);

    return true;
}

KT_TEST(QueueClearTest) {
    kronos::Queue<int> queue;

    queue.Clear();
    queue.Push(10);
    queue.Push(11);
    queue.Clear();

    KT_ASSERT(queue.Size() == 0);

    queue.Push(12);

    int popped = 0;
    KT_ASSERT(queue.Pop(&popped));
    KT_ASSERT(popped == 12);

    return true;
}

KT_TEST(QueueCapacityTest) {
    kronos::Queue<int> queue;

    queue.Clear();
    queue.Push(10);

    KT_ASSERT(queue.Length() == 10);

    return true;
}