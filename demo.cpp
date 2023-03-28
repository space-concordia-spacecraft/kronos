#include "kronos.h"

using namespace kronos;

static void Start(void* data);

int main() {
    // Task has to be created for Reliance Edge to be initialized properly
    xTaskCreate(
        Start,
        "MAIN",
        KS_COMPONENT_STACK_SIZE_LARGE,
        nullptr,
        KS_COMPONENT_PRIORITY_HIGH,
        nullptr
    );

    // START FREERTOS
    vTaskStartScheduler();
    return 1;
}

static void Start(void* data) {
    // INITIALIZE ATMEL DRIVERS
    atmel_start_init();

    // INITIALIZE FRAMEWORK
    Framework::CreateInstance();

    // INITIALIZE ALL DRIVERS
    Framework::CreateDescriptor<KsUsart>(KS_DESC_UART_COMMS, &USART_UHF);

    // ADD MODULES
    Framework::AddModule<FileManagerModule>();
    Framework::AddModule<SchedulerModule>();
    Framework::AddModule<WorkerModule>();
    Framework::AddModule<CommunicationHandlerModule>();
    Framework::AddModule<ParamsModule>();

    // INITIALIZE MODULES
    Framework::InitModules();

    // START FRAMEWORK
    Framework::Start();

    vTaskDelete(nullptr);
}
