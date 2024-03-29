#include "ks_profiler.h"
#include "ks_file.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Profiler);

    Profiler::Profiler() : ComponentQueued("CQ_PROFILER"), m_File(KS_PROFILER_FILE_PATH) {}

    KsResult Profiler::ProcessEvent(const EventMessage& eventMessage) {
        switch (eventMessage.eventCode) {
            case ks_event_save_param:
                SaveProfiles();
                break;
        }
        return ks_success;
    }

    KsResult Profiler::_Update(const String& scopeName, const String& path, TickType_t start, TickType_t end) {
        auto intervalMs = (start - end) * portTICK_RATE_MS;

        TaskStatus_t currentTaskDetails;
        vTaskGetInfo(nullptr, &currentTaskDetails, pdTRUE, eRunning);

        if (m_ProfiledFunctions.contains(scopeName) && m_ProfiledFunctions[scopeName].interval > intervalMs)
            return ks_success;

        m_ProfiledFunctions[scopeName] = {
            .path = path,
            .interval = intervalMs,
            .stackSizeRemaining = currentTaskDetails.usStackHighWaterMark
        };

        return ks_success;
    }

    KsResult Profiler::SaveProfiles() {
        if (!m_File.IsOpen())
            return ks_success;

        for (const auto& [scopeName, profileInfo]: m_ProfiledFunctions) {
            m_File.Write(scopeName.c_str(), scopeName.size());
            m_File.Write((uint8_t*)&profileInfo, sizeof(profileInfo));
        }

        // Log the run time tasks as well? https://www.freertos.org/a00021.html#vTaskGetRunTimeStats
        return ks_success;
    }
}
