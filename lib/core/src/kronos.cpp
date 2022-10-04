#include "kronos.h"

#include <stdarg.h>
#include <stdio.h>

#include <fcntl.h>
#include <errno.h>

#include <vector>

extern "C" {

    void vApplicationMallocFailedHook(void) {
        configASSERT((volatile void*) nullptr);
    }

    void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName) {
        configASSERT((volatile void*) nullptr);
    }

    void vApplicationIdleHook(void) {}

    void vApplicationTickHook(void) {}

}

