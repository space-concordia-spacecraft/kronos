#pragma once

/* Kronos includes. */
#include "ks_component_queued.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Library includes. */
#include "asf.h"

/* Task Priority */
#define KS_TASK_PRIORITY_HIGH      ( tskIDLE_PRIORITY + 3 )
#define KS_TASK_PRIORITY_MEDIUM    ( tskIDLE_PRIORITY + 2 )
#define KS_TASK_PRIORITY_LOW       ( tskIDLE_PRIORITY + 1 )

namespace kronos {

    class ComponentActive : public ComponentQueued {

    public:
        ComponentActive(const String& name, size_t stackSize, uint16_t priority);

        void Init() override;
        void Destroy() override;

        void Run();

    private:
        size_t m_StackSize;
        uint16_t m_Priority;

        TaskHandle_t m_Task = nullptr;

        static void Start(void* data);
    };
}

