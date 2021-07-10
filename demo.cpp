#include "kronos.h"

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