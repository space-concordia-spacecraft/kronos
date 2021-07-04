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
#define KS_COMPONENT_STACK_SIZE_LARGE   2048
#define KS_COMPONENT_STACK_SIZE_MEDIUM  1024
#define KS_COMPONENT_STACK_SIZE_SMALL   512

namespace kronos {

    class ComponentActive : public ComponentQueued {
    public:
        ComponentActive(const String& name, size_t stackSize, uint16_t priority);

        virtual void Init() override;
        virtual void Destroy() override;

        [[noreturn]] void Run();

    private:
        size_t m_StackSize;
        uint16_t m_Priority;

        TaskHandle_t m_Task = nullptr;

        static void Start(void* data);
    };

}

