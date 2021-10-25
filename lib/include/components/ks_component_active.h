#pragma once

// Kronos includes
#include "ks_component_queued.h"
#include "ks_bus.h"

// Kernel includes
#include "FreeRTOS.h"
#include "task.h"

// Microchip ASF
#include "asf.h"

// Task Priorities
#define KS_COMPONENT_PRIORITY_HIGH      ( tskIDLE_PRIORITY + 3 )
#define KS_COMPONENT_PRIORITY_MEDIUM    ( tskIDLE_PRIORITY + 2 )
#define KS_COMPONENT_PRIORITY_LOW       ( tskIDLE_PRIORITY + 1 )

// Stack sizes
#define KS_COMPONENT_STACK_SIZE_XLARGE  configMINIMAL_STACK_SIZE * 8
#define KS_COMPONENT_STACK_SIZE_LARGE   configMINIMAL_STACK_SIZE * 4
#define KS_COMPONENT_STACK_SIZE_MEDIUM  configMINIMAL_STACK_SIZE * 2
#define KS_COMPONENT_STACK_SIZE_SMALL   configMINIMAL_STACK_SIZE

namespace kronos {

    /// Class implementation for Active components.
    ///
    /// Active components inherit directly from the queue. The main difference is that active components have a thread that is constantly processing events from the queue.
    class ComponentActive : public ComponentQueued {
    public:

        /// @copydoc
        explicit ComponentActive(const String& name, size_t stackSize = KS_COMPONENT_STACK_SIZE_SMALL, uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

        /// @copydoc
        KsResult Init() override;

        /// @copydoc
        KsResult Destroy() override;

        /// Function containing main loop for the event processing thread.
        [[noreturn]] void Run();

        /// @copydoc
        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        /// Stack size provided to the task.
        size_t m_StackSize;

        /// Priority of the task.
        uint16_t m_Priority;

        /// Task handle used to manipulate the task created by the FreeRTOS API.
        TaskHandle_t m_Task = nullptr;

        /**
         * Function that takes care of starting the thread.
         * @param pointer to the class that starts the thread.
         */
        static void Start(void* data);
    };

}

