#include "ks_driver.h"

namespace kronos {

    // Driver Base
    Driver::Driver(const std::string& name):m_Name(name){}

    // GPIO
    DriverGPIO::DriverGPIO(
            const std::string& name,
            uint8_t pin,
            gpio_direction gpioDirection,
            bool gpioLevel,
            gpio_pull_mode gpioPullMode,
            uint32_t gpioFunction
            ):Driver(name),
            m_Pin(pin),
            m_Direction(gpioDirection),
            m_Level(gpioLevel),
            m_PullMode(gpioPullMode),
            m_Function(gpioFunction){}

    void DriverGPIO::Init() {
        gpio_set_pin_direction(m_Pin, m_Direction);
        gpio_set_pin_level(m_Pin, m_Level);
        gpio_set_pin_pull_mode(m_Pin, m_PullMode);
        gpio_set_pin_function(m_Pin, m_Function);
    }

    void DriverGPIO::Read() {

    }

    void DriverGPIO::Write() {

    }

    // UART
    DriverUART::DriverUART(const std::string& name,
                           uint32_t peripheralId,
                           uint32_t rxPin,
                           uint32_t rxPinFunction,
                           uint32_t txPinFunction,
                           uint32_t txPin,
                           void *const baseAddress) :
            Driver(name),
            m_PeripheralId(peripheralId),
            m_RxPin(rxPin),
            m_RxPinFunction(rxPinFunction),
            m_TxPin(txPin),
            m_TxPinFunction(txPinFunction),
            m_BaseAddress(baseAddress) {}

    void DriverUART::Init() {
        _pmc_enable_periph_clock(m_PeripheralId);

        usart_sync_init(&m_Descriptor, m_BaseAddress, _uart_get_usart_sync());

        gpio_set_pin_function(m_RxPin, m_RxPinFunction);
        gpio_set_pin_function(m_TxPin, m_TxPinFunction);
    }

    void DriverUART::Read() {

    }

    void DriverUART::Write() {

    }

    // CAN
    DriverCAN::DriverCAN(const std::string& name,
                         uint32_t peripheralId,
                         uint32_t rxPin,
                         uint32_t rxPinFunction,
                         uint32_t txPinFunction,
                         uint32_t txPin,
                         void *const baseAddress) :
            Driver(name),
            m_PeripheralId(peripheralId),
            m_RxPin(rxPin),
            m_RxPinFunction(rxPinFunction),
            m_TxPin(txPin),
            m_TxPinFunction(txPinFunction),
            m_BaseAddress(baseAddress) {}

    void DriverCAN::Init() {
        _pmc_enable_periph_clock(m_PeripheralId);
        gpio_set_pin_function(m_RxPin, m_RxPinFunction);
        gpio_set_pin_function(m_TxPin, m_TxPinFunction);
        can_async_init(&m_Descriptor, m_BaseAddress);
    }

    void DriverCAN::Read() {

    }

    void DriverCAN::Write() {

    }
}