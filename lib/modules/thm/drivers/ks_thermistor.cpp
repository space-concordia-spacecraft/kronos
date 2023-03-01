#include "ks_thermistor.h"

namespace kronos {
    Thermistor::Thermistor(float A, float B, float C, float D, IoDriver* driver) :
        m_CoefficientA(A), m_CoefficientB(B), m_CoefficientC(C), m_CoefficientD(D), m_Driver(driver) {
    }

    float Thermistor::GetTemperature() {
        return 0;
    }
}
