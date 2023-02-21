#pragma once

#include "ks_component_queued.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "ks_logger.h"

namespace kronos {
    struct ThermistorBoundaries {
        IoDriver* thermistor;
        int16_t lowerBoundary;
        int16_t upperBoundary;
    };

    class Heater : public ComponentQueued {
    public:
        Heater(const std::string& name, uint8_t pin);

        void ProcessEvent(const EventMessage& message) override;
        void AddThermistor(IoDriver* thermistor, int16_t lowerBoundary, int16_t upperBoundary);

    private:
        void Update();

    private:
        List<ThermistorBoundaries> m_Thermistors;
        uint8_t m_Pin;
        bool m_IsActive;
    };

}