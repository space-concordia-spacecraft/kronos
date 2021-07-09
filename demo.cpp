#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "board.h"
#include "asf.h"

#include "console.h"
#include "ks_logger.h"

[[noreturn]] void pvTaskBlink(void * pvParam) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    TickType_t lastTime = xLastWakeTime;
    size_t blinkCount = 0;
    while (true) {
        if (xTaskGetTickCount() - lastTime >= pdMS_TO_TICKS(1000)) {
            printf("blinks per second: %zu \n", blinkCount);
            blinkCount = 0;
            lastTime = xTaskGetTickCount();
        }
        ioport_toggle_pin_level(LED0_GPIO);
        blinkCount++;
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

int main() {
    sysclk_init();
    board_init();
    console_init();

    xTaskCreate(pvTaskBlink, "BLINK", configMINIMAL_STACK_SIZE, nullptr, 1, nullptr);



    vTaskStartScheduler();
    kronos::ComponentLogger thelogger("logger","/Logs");
    while (true);
}

extern "C" {

void vApplicationMallocFailedHook(void) {
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

    /* Force an assert. */
    configASSERT((volatile void*) nullptr);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName) {
    (void) pcTaskName;
    (void) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */

    /* Force an assert. */
    configASSERT((volatile void*) nullptr);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void) {
    volatile size_t xFreeHeapSpace;

    /* This is just a trivial example of an idle hook.  It is called on each
    cycle of the idle task.  It must *NOT* attempt to block.  In this case the
    idle task just queries the amount of FreeRTOS heap that remains.  See the
    memory management section on the http://www.FreeRTOS.org web site for memory
    management options.  If there is a lot of heap memory free then the
    configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
    RAM. */
    xFreeHeapSpace = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    (void) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {}

}