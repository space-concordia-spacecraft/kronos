#include "ks_heater.h"

namespace kronos {
    Heater::Heater(const std::string& name, uint8_t pin) : ComponentQueued(name), m_Pin(pin) {
    }

    void Heater::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_update_heater:
                Update();
                break;
        }
    }

    void Heater::AddThermistor(IoDriver* thermistor, int16_t lowerBoundary, int16_t upperBoundary) {
        m_Thermistors.push_back({
            .thermistor = thermistor,
            .lowerBoundary = lowerBoundary,
            .upperBoundary = upperBoundary
        });
    }

    void Heater::Update() {
        for(auto& [thermistor, lowerBoundary, upperBoundary]: m_Thermistors) {
            auto test = 1; // TODO: replace with the temp being read
            if(m_IsActive && test > upperBoundary) {
                m_IsActive = false;
            } else if(!m_IsActive && test < lowerBoundary) {
                m_IsActive = true;
            }
        }
    }

}

