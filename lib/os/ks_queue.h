#pragma once

#include "ks_error_codes.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "ks_config.h"

#include <memory>

#define KS_QUEUE_DEFAULT_WAIT_TIME pdMS_TO_TICKS(200)

namespace kronos {
    //! \class Template for the Queue Class
    //! \tparam T Generic Template for Queue
    template<typename T>
    class Queue {
    public:
        virtual ~Queue() = default;

        //! \brief Enqueues element into the queue.
        //!
        //! \param element the element object to insert into the queue
        //! \param ticksToWait ticks to wait before throwing an error
        //! \return true if the operation was successful
        virtual KsResultType Push(const T& element, KsTickType ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) = 0;

        //! \brief Dequeues element from the queue.
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait clock cycles to wait
        //! \return Returns if the element was popped successfully.
        virtual KsResultType Pop(T* pElement, KsTickType ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) = 0;

        //! \brief Function to Check if the element exists
        //!
        //! \param pElement pointer to the element
        //! \param ticksToWait
        //! \return pdTrue if the value exists, pdFalse otherwise
        virtual KsResultType Peek(T* pElement, KsTickType ticksToWait = KS_QUEUE_DEFAULT_WAIT_TIME) const = 0;

        //! \brief Function to Clear the Queue
        //!
        //! \return pdPASS to show teh queue is cleared
        virtual KsResultType Clear() = 0;

        //! \brief Getter function for the length of the Queue
        //! \return size of the Queue
        [[nodiscard]] virtual size_t Length() const = 0;

        //! \brief Function to find the amount of spaces remaining.
        //! \return Amount of free blocks remaining in the Queue.
        [[nodiscard]] virtual size_t Size() const = 0;

        static std::shared_ptr<Queue<T>> Create();
    };
}

#ifdef KS_FREERTOS_API
#include "ks_queue_freertos.h"
#elif defined(KS_WINDOWS_API)
#include "ks_queue_windows.h"
#endif

namespace kronos {
    //! \brief
    template<typename T>
    std::shared_ptr<Queue<T>> Queue<T>::Create(){
#ifdef KS_FREERTOS_API
        return std::make_shared<QueueFreeRTOS<T>>();
#elif defined(KS_WINDOWS_API)
        return std::make_shared<QueueWindows<T>>();
#endif
        return nullptr;
    }
}
