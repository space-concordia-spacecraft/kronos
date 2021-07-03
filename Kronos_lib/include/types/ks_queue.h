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

        bool push(V element, TickType_t ticksToWait) {
            return xQueueSend(m_Queue, element, ticksToWait);
        }

        bool push(V element) {
            return push(element, 0);
        }

        bool pop(V *buffer, TickType_t ticksToWait) {
            return xQueueReceive( m_Queue, &( buffer ), ticksToWait);
        }

        bool pop(V *buffer) {
            return pop(buffer, 0);
        }

        bool peek(V *buffer, TickType_t ticksToWait) {
            return ( xQueuePeek( m_Queue, &( buffer ), ticksToWait);
        }

        bool peek(V *buffer) {
            return peek(buffer, 0);
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