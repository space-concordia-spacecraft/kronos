#include "FreeRTOS.h"

#define KS_QUEUE_DEFAULT_WAIT_TIME pdMS_TO_TICKS(200)

namespace kronos {

    //! \class Template for the Queue Class
    //! \tparam T Generic Template for Queue
    template<typename T>
    class QueueFreeRTOS : public Queue<T> {
    public:
        //! \brief Parametrized Constructor to create a Queue with size 10
        explicit QueueFreeRTOS(size_t length = 40) : m_Length(length), m_Queue(xQueueCreate(length, sizeof(T))) {}

        ~QueueFreeRTOS() override { vQueueDelete(m_Queue); };

        //! \brief Enqueues element into the queue.
        //!
        //! \param element the element object to insert into the queue
        //! \param ticksToWait ticks to wait before throwing an error
        //! \return true if the operation was successful
        KsResultType Push(const T& element, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) override {
            return xQueueSend(m_Queue, &element, ticksToWait);
        };

        //! \brief Dequeues element from the queue.
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait clock cycles to wait
        //! \return Returns if the element was popped successfully.
        BaseType_t Pop(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) override {
            return xQueueReceive(m_Queue, pElement, ticksToWait);
        }

        //! \brief Function to Check if the element exists
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait
        //! \return pdTrue if the value exists, pdFalse otherwise
        BaseType_t Peek(T* pElement, TickType_t ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) const override {
            return xQueuePeek(m_Queue, pElement, ticksToWait);
        }

        //! \brief Function to Clear the Queue
        //!
        //! \return pdPASS to show teh queue is cleared
        BaseType_t Clear() override {
            return xQueueReset(m_Queue);
        }

        //! \brief Getter function for the length of the Queue
        //! \return size of the Queue
        [[nodiscard]] size_t Length() const override {
            return m_Length;
        }

        //! \brief Function to find the amount of spaces remaining.
        //! \return Amount of free blocks remaining in the Queue.
        [[nodiscard]] size_t Size() const override {
            return m_Length - uxQueueSpacesAvailable(m_Queue);
        }

    private:
        //! Length of the Queue.
        const size_t m_Length;
        //! Queue Data structure.
        QueueHandle_t m_Queue;
    };
}