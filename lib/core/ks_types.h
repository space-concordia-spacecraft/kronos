#pragma once

#include "ks_config.h"


// API TYPES
#ifdef KS_FREERTOS_API

#include "FreeRTOS.h"

typedef BaseType_t KsResultType;
typedef TickType_t KsTickType;
#endif

// DRIVER TYPES
#ifdef KS_ASF_DRIVERS

#include "hal_gpio.h"

typedef gpio_pull_mode KsGpioPullMode;
typedef gpio_direction KsGpioDirection;

#elif defined(KS_POSIX_SIM)
typedef bool KsGpioPullMode;
typedef bool KsGpioDirection;
#endif

//! \typedef Type that tells you the result of
typedef void* KsCmdResult;
typedef uint16_t KsOpcode;

#define KS_CMDRESULT_NORETURN ((KsCmdResult) nullptr)
