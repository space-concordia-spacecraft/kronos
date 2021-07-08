#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "asf.h"

#define KS_QUEUE_DEFAULT_WAIT_TIME ( 1000 / portTICK_PERIOD_MS )

namespace kronos {

    template<typename T>
    class Queue {
    public:
        explicit Queue(size_t length = 10);
        ~Queue();

        BaseType_t Push(const T& element, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME);

        BaseType_t Pop(T* buffer, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME);

        BaseType_t Peek(T* buffer, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) const;

        BaseType_t Clear();

        size_t Length() const;

        size_t Size() const;

    private:
        const size_t m_Length;
        QueueHandle_t m_Queue;
    };

}