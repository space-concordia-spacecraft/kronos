#include "ks_sched_module.h"
#include "ks_scheduler.h"

namespace kronos {

    SchedModule::SchedModule() : Module("M_SCHED") {}

    void SchedModule::Init() const {
        Scheduler::CreateInstance();
    }

}