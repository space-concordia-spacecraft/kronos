#include "ks_thermistor.h"
#include "math.h"

namespace kronos {
    Thermistor::Thermistor(float A, float B, float C, float D, uint16_t materialConstant, uint16_t refResistance) :
        m_CoefficientA(A), m_CoefficientB(B), m_CoefficientC(C), m_CoefficientD(D), m_MaterialConstant(materialConstant), m_RefResistance(refResistance) {
    }

    float Thermistor::GetTemperature(float resistance, bool doAccurate) {
        if(doAccurate)
            return 1/(m_CoefficientA + (m_CoefficientB*log(resistance/m_RefResistance)) + (m_CoefficientC*pow(log(resistance/m_RefResistance),2)) + (m_CoefficientD*pow(log(resistance/m_RefResistance), 3)));
        return 1/((1/298.15) + (log(resistance/m_RefResistance)/m_MaterialConstant));
    }
}
