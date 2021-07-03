#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "portmacro.h"

namespace kronos {
    template<typename V>
    class Queue {
    public:
        Queue(UBaseType_t length = 5): m_Length(length), m_Queue(xQueueCreate( length, sizeof( V ) )){ }

        ~Queue() {
            vQueueDelete(m_Queue);
        }

        BaseType_t push(V element, TickType_t ticksToWait = 0) {
            return xQueueSend(m_Queue, element, ticksToWait);
        }

        BaseType_t pop(V *buffer, TickType_t ticksToWait = 0) {
            return xQueueReceive( m_Queue, &( buffer ), ticksToWait);
        }

        BaseType_t peek(V *buffer, TickType_t ticksToWait = 0) {
            return xQueuePeek( m_Queue, &( buffer ), ticksToWait);
        }

        void clear() {
            xQueueReset(m_Queue);
        }

        UBaseType_t getLength() { return m_Length; }

    private:
        QueueHandle_t m_Queue;
        UBaseType_t m_Length;
    };
}