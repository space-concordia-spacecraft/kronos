#pragma once

#include "FreeRTOS.h"
#include "ks_queue.h"

#define KS_QUEUE_DEFAULT_WAIT_TIME pdMS_TO_TICKS(200)

namespace kronos {
    template<typename T>
    Queue<T>::Queue(size_t length) : m_Length(length), m_Queue(xQueueCreate(length, sizeof(T))) {}

    template<typename T>
    Queue<T>::~Queue() { vQueueDelete(m_Queue); }

    template<typename T>
    BaseType_t Queue<T>::Push(const T& element, TickType_t ticksToWait) {
        return xQueueSend(m_Queue, &element, ticksToWait);
    }

    template<typename T>
    BaseType_t Queue<T>::Pop(T* pElement, TickType_t ticksToWait) {
        return xQueueReceive(m_Queue, pElement, ticksToWait);
    }

    template<typename T>
    BaseType_t Queue<T>::Peek(T* pElement, TickType_t ticksToWait) const {
        return xQueuePeek(m_Queue, pElement, ticksToWait);
    }

    template<typename T>
    BaseType_t Queue<T>::Clear() {
        return xQueueReset(m_Queue);
    }

    template<typename T>
    size_t Queue<T>::Length() const {
        return m_Length;
    }

    template<typename T>
    size_t Queue<T>::Size() const {
        return Length() - uxQueueSpacesAvailable(m_Queue);
    }

}