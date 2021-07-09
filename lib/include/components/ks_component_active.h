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

    class ComponentActive : public ComponentQueued {
    public:
        ComponentActive(const String& name, size_t stackSize = KS_COMPONENT_STACK_SIZE_SMALL, uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

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

