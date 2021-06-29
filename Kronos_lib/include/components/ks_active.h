#pragma once

#include "../component.h"
#include <task.h>
#include <FreeRTOS.h>


namespace kronos {

    class Active : private Component {
        Active();
    public:
        void Start() {

        }

        void Stop();
        void Run();
    private:
        char * m_Id;
        int m_Priority;
        int m_StackSize;


    };
}

