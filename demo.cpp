#include "kronos.h"
#include "ks_worker_manager.h"
#include "atmel_start.h"

using namespace kronos;

static void Start(void* data);

int main() {
    // Task has to be created for Reliance Edge to be initialized properly
    xTaskCreate(
        Start,
        "MAIN",
        KS_COMPONENT_STACK_SIZE_XLARGE,
        nullptr,
        KS_COMPONENT_PRIORITY_HIGH,
        nullptr
    );

    // START FREERTOS
    vTaskStartScheduler();
    return ks_success;
}

static void Start(void* data) {
    // INITIALIZE ATMEL DRIVERS
    atmel_start_init();

    // INITIALIZE FRAMEWORK
    Framework::CreateInstance();

    // INITIALIZE ALL DRIVERS
    Framework::CreateDriver<KsUsart>("D_CMD", &USART_0);

    // ADD MODULES
    Framework::AddModule<FsModule>();
    Framework::AddModule<SchedModule>();
    Framework::AddModule<WorkerModule>();
    Framework::AddModule<CmdModule>();
    Framework::AddModule<LogModule>();
    Framework::AddModule<ParamsModule>();
    Framework::AddModule<ClkModule>();

    // INITIALIZE MODULES
    Framework::InitModules();

    // START FRAMEWORK
    Framework::Start();

    vTaskDelete(nullptr);
}
