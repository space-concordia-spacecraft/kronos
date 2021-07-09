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

    kronos::ComponentLogger thelogger("logger","/Logs");

    xTaskCreate(pvTaskBlink, "BLINK", configMINIMAL_STACK_SIZE, nullptr, 1, nullptr);

    vTaskStartScheduler();

    while (true);
}

extern "C" {

void vApplicationMallocFailedHook(void) {
    configASSERT((volatile void*) nullptr);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName) {
    configASSERT((volatile void*) nullptr);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void) {}

/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {}

}