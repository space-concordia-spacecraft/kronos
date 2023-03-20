#pragma once

/**
 * Thermistor being implemented on dev board: NTHS0603N02N1002JE
 * Thermistor Calculator to get coefficients: https://www.vishay.com/en/thermistors/ntc-rt-calculator/
 * All formulas: https://www.vishay.com/docs/29053/ntcappnote.pdf
 */

namespace kronos {
    class Thermistor {
    /**
     * Public
     *  - Thermistor(double A, double B, double C, double D, uint16_t Beta, (not sure) Alpha, bool isAccurate = false, uint16_t referenceResistance = 10000)
     *  - ErrorOr<float> GetTemperature(uint16_t resistor) returns temperature
     *  - ErrorOr<void> SetAccurate(bool isAccurate) returns void
     *  - ErrorOr<bool> GetAccurate() returns isAccurate
     *
     * Private member variables:
     *  - bool IsAccurate to choose either Beta or long formula with other coefficients
     *  - uint16_t Reference Resistance at 25 deg C
     *  - uint16_t Beta (Material constant)
     *  - (not sure) Alpha (Temperature coefficient of resistance)
     *  - float A
     *  - float B
     *  - float C
     *  - float D
     */
    };
}