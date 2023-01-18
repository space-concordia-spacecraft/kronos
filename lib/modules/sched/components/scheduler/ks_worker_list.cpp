#include "ks_scheduler.h"

namespace kronos {
    Map <KsIdType, WorkerConfig> Scheduler::s_WorkerList{
        {ks_worker_2s,  {.tickRate=20, .eventCode=ks_event_scheduler_tick}},
        {ks_worker_10s, {.tickRate=200, .eventCode=ks_event_scheduler_tick}}
    };
}