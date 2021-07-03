#pragma once

/* Kronos includes. */
#include "ks_component_queue.h"
#include "ks_queue.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Library includes. */
#include "board.h"
#include "asf.h"

/* Task Priority */
#define TASK_PRIORITY_HIGH	( tskIDLE_PRIORITY + 3 )
#define	TASK_PRIORITY_MEDIUM ( tskIDLE_PRIORITY + 2 )
#define TASK_PRIORITY_LOW ( tskIDLE_PRIORITY + 1 )

namespace kronos {

    class ActiveComponent : private QueuedComponent {
        ActiveComponent(char * id, unsigned short stackSize, int priority);
        ~ActiveComponent();
    public:
        void Init();
        void Stop();
        void Run();

        UBaseType_t getPriority();
        void setPriority(UBaseType_t prio);

    private:
        char * m_Id;
        int m_Priority;
        uint16_t m_StackSize;

        bool m_Running;

        TaskHandle_t m_Task;

        static void Start(void *);
    };
}

