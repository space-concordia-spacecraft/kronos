#include "ks_scheduler.h"

namespace kronos {
    Map <KsIdType, WorkerConfig> Scheduler::s_WorkerList{
        {ks_worker_main, {
                             .tickRate=0,
                             .eventCode=ks_event_empty_queue,
                             .stackSize=KS_COMPONENT_STACK_SIZE_XLARGE,
                             .priority=KS_COMPONENT_PRIORITY_HIGH}},
        {ks_worker_2s,   {
                             .tickRate=20,
                             .eventCode=ks_event_scheduler_tick
                         }},
        {ks_worker_10s,  {
                             .tickRate=200,
                             .eventCode=ks_event_scheduler_tick
        }}
    };
}