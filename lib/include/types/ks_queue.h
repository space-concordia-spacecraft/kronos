#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "asf.h"

#define KS_QUEUE_DEFAULT_WAIT_TIME ( 1000 / portTICK_PERIOD_MS )

namespace kronos {

    template<typename T>
    class Queue {
    public:
        explicit Queue(size_t length = 10) : m_Length(length), m_Queue(xQueueCreate(length, sizeof(T))) {}

        ~Queue() { vQueueDelete(m_Queue); }

        BaseType_t Push(const T& element, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) {
            return xQueueSend(m_Queue, &element, ticksToWait);
        }

        BaseType_t Pop(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) {
            return xQueueReceive(m_Queue, pElement, ticksToWait);
        }

        BaseType_t Peek(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) const {
            return xQueuePeek(m_Queue, pElement, ticksToWait);
        }

        BaseType_t Clear() {
            return xQueueReset(m_Queue);
        }

        size_t Length() const {
            return m_Length;
        }

        size_t Size() const {
            return Length() - uxQueueSpacesAvailable(m_Queue);
        }

    private:
        const size_t m_Length;
        QueueHandle_t m_Queue;
    };

}