#pragma once

#include "FreeRTOS.h"
#include "queue.h"

#define KS_QUEUE_DEFAULT_WAIT_TIME 1000
#define KS_QUEUE_DEFAULT_SIZE      40

namespace kronos {
    //! \class Template for the Queue Class
    //! \param T Generic Template for Queue
    template<typename T>
    class Queue {
    public:
        explicit Queue(QueueHandle_t queue, size_t length = 10) : m_Length(length), m_Queue(queue) {}

        ~Queue() { vQueueDelete(m_Queue); }

        static Ref<Queue<T>> Create(size_t length = KS_QUEUE_DEFAULT_SIZE) {
            QueueHandle_t handle = xQueueCreate(length, sizeof(T));

            // if(handle == 0) KS_THROW(ks_error_queue_create);

            return CreateRef<Queue<T>>(handle, length);
        }

        //! \brief Enqueues element into the queue.
        //!
        //! \param element the element object to insert into the queue
        //! \param ticksToWait ticks to wait before throwing an error
        //! \return true if the operation was successful
        KsResult Push(const T& element, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) {
            if(xQueueSend(m_Queue, &element, ticksToWait) != pdPASS) KS_THROW(ks_error_queue_push);

            return ks_success;
        }

        //! \brief Dequeues element from the queue.
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait clock cycles to wait
        //! \return Returns if the element was popped successfully.
        KsResult Pop(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) {
            if(xQueueReceive(m_Queue, pElement, ticksToWait) != pdPASS) KS_THROW(ks_error_queue_pop);

            return ks_success;
        }

        //! \brief Function to Check if the element exists
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait
        //! \return pdTrue if the value exists, pdFalse otherwise
        KsResult Peek(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) const {
            if(xQueuePeek(m_Queue, pElement, ticksToWait) != pdPASS) KS_THROW(ks_error_queue_peek);

            return ks_success;
        }

        //! \brief Function to Clear the Queue
        //!
        //! \return pdPASS to show the queue is cleared
        KsResult Clear() {
            if(xQueueReset(m_Queue) != pdPASS) KS_THROW(ks_error_queue_clear);

            return ks_success;
        }

        //! \brief Getter function for the length of the Queue
        //! \return size of the Queue
        [[nodiscard]] size_t Length() const {
            return m_Length;
        }

        //! \brief Function to find the amount of spaces remaining.
        //! \return Amount of free blocks remaining in the Queue.
        [[nodiscard]] size_t Size() const {
            return Length() - uxQueueSpacesAvailable(m_Queue);
        }

    private:
        //! Length of the Queue.
        const size_t m_Length;
        //! Queue Data structure.
        QueueHandle_t m_Queue;
    };
}