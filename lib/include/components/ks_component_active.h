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

        /**
         *
         * @param name
         * @param stackSize
         * @param priority
         */
        explicit ComponentActive(const String& name, size_t stackSize = KS_COMPONENT_STACK_SIZE_SMALL, uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

        void Init() override;
        void Destroy() override;

        [[noreturn]] void Run();

        /**
         * ProcessCommand() takes care of directing the event message, being sent to the component, into the correct handler.
         * @param message - EventMessage struct containing the opcode, data, and return bus
         * @return Whether the function was successful or not. It will return KS_SUCCESS if it worked, otherwise it will return an error code.
         */
        KsCmdResult ProcessCommand(const CommandMessage& message) override;

    private:
        /// Stack size provided to the task.
        size_t m_StackSize;

        /// Priority of the task.
        uint16_t m_Priority;

        /// Task handle used to manipulate the task created by the FreeRTOS API.
        TaskHandle_t m_Task = nullptr;

        /**
         * Start() is the static function that allows the FreeRTOS task to run.
         * @param data - void ptr used to obtain this class in order to be able to call the Run() function.
         */
        static void Start(void* data);
    };

}

