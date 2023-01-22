#include "ks_scheduler.h"

namespace kronos {
    Map <KsIdType, WorkerConfig> Scheduler::s_PublisherList{
        {
            ks_worker_main,
            {
                .tickRate=0,
                .eventCode=ks_event_empty_queue,
                .priority=KS_COMPONENT_PRIORITY_HIGH
            }
        },
        {
            ks_worker_10s,
            {
                .tickRate=200,
                .eventCode=ks_event_scheduler_tick
            }
        }
    };
}