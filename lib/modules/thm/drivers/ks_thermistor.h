#pragma once

#include "ks_io.h"

namespace kronos {
    class Thermistor {
    public:
        Thermistor(float A, float B, float C, float D, IoDriver* driver);
        float GetTemperature();

    private:
        float m_CoefficientA;
        float m_CoefficientB;
        float m_CoefficientC;
        float m_CoefficientD;

        IoDriver* m_Driver;
    };
}
