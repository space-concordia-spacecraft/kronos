#pragma once

/* Kronos includes. */
#include "ks_component.h"
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

    class Active : private Component {
        Active(char * id, unsigned short stackSize, int priority);
        ~Active();
    public:
        void Start();
        void Stop();
        void Run();



    private:
        char * m_Id;
        int m_Priority;
        uint16_t m_StackSize;

        bool m_Running;

        kronos::Queue<int> m_Queue;

        static void StartTask(void *);
    };
}

