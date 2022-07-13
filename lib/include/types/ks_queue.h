#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "asf.h"

#define KS_QUEUE_DEFAULT_WAIT_TIME pdMS_TO_TICKS(200)

namespace kronos {
    //! \class Template for the Queue Class
    //! \tparam T Generic Template for Queue
    template<typename T>
    class Queue {
    public:
        //! \brief Parametrized Constructor to create a Queue with size 10
        explicit Queue(size_t length = 10) : m_Length(length), m_Queue(xQueueCreate(length, sizeof(T))) {}

        ~Queue() { vQueueDelete(m_Queue); }

        //! \brief Enqueues element into the queue.
        //!
        //! \param element the element object to insert into the queue
        //! \param ticksToWait ticks to wait before throwing an error
        //! \return true if the operation was successful
        BaseType_t Push(const T& element, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) {
            return xQueueSend(m_Queue, &element, ticksToWait);
        }

        //! \brief Dequeues element from the queue.
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait clock cycles to wait
        //! \return Returns if the element was popped successfully.
        BaseType_t Pop(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) {
            return xQueueReceive(m_Queue, pElement, ticksToWait);
        }

        //! \brief Function to Check if the element exists
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait
        //! \return pdTrue if the value exists, pdFalse otherwise
        BaseType_t Peek(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) const {
            return xQueuePeek(m_Queue, pElement, ticksToWait);
        }

        //! \brief Function to Clear the Queue
        //!
        //! \return pdPASS to show teh queue is cleared
        BaseType_t Clear() {
            return xQueueReset(m_Queue);
        }

        //! \brief Getter function for the length of the Queue
        //! \return size of the Queue
        size_t Length() const {
            return m_Length;
        }

        //! \brief Function to find the amount of spaces remaining.
        //! \return Amount of free blocks remaining in the Queue.
        size_t Size() const {
            return Length() - uxQueueSpacesAvailable(m_Queue);
        }

    private:
        //! Length of the Queue.
        const size_t m_Length;
        //! Queue Data structure.
        QueueHandle_t m_Queue;
    };

}