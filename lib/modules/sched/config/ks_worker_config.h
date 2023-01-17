#pragma once

namespace kronos {
    enum KsWorker: KsWorkerIdType {
        ks_worker_2s,
        ks_worker_10s
    };

    struct WorkerConfig {
        uint16_t tickRate,
        KsEventCodeType eventCode
    };

    static Map <KsWorkerIdType, uint16_t> s_WorkerList;
}

