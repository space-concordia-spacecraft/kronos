#pragma once

#include "ks_io.h"

namespace kronos {
    class Thermistor {
    public:
        Thermistor(float A, float B, float C, float D, uint16_t materialConstant, uint16_t refResistance);
        float GetTemperature(float resistance, bool doAccurate = false);

    private:
        float m_CoefficientA;
        float m_CoefficientB;
        float m_CoefficientC;
        float m_CoefficientD;

        uint16_t m_MaterialConstant;
        uint16_t m_RefResistance;
    };
}
