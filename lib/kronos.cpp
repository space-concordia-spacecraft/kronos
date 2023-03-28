#include "ks_freertos.h"

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
