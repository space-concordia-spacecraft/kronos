#pragma once

// Kronos includes
#include "ks_component_queued.h"

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
    /**
     * A class that implements the logic for Active components.
     *
     * An Active component works the same way as the queued component except that it has it's own thread that clears the queue.
     * When creating an active component, it's important to implement the ProcessCommand() function and then call ComponentActive::ProcessCommand() as the base component takes care of the health checks of the thread.
     */
    class ComponentActive : public ComponentQueued {
    public:

        explicit ComponentActive(const String& name, size_t stackSize = KS_COMPONENT_STACK_SIZE_SMALL, uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

        void Init() override;
        void Destroy() override;

        [[noreturn]] void Run();

        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        /// Stack size provided to the task.
        size_t m_StackSize;

        /// Priority of the task.
        uint16_t m_Priority;

        /// Task handle used to manipulate the task created by the FreeRTOS API.
        TaskHandle_t m_Task = nullptr;

        static void Start(void* data);
    };

}

