#pragma once

// Config
#include "ks_config.h"

// Standard Library
#include <cstdint>
#include <climits>
#include <cstdarg>
#include <cstring>

typedef int32_t KsResultType;
typedef uint32_t KsTickType;
typedef uint32_t KsIdType;

#include <memory>
#include <algorithm>
#include <variant>
#include <type_traits>
#include <numeric>
#include <typeinfo>

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <regex>
#include <any>
#include <span>
#include <sstream>

// KRONOS TYPES AND MACROS
#include "ks_macros.h"
#include "ks_error_codes.h"
#include "ks_event_codes.h"

// API TYPES
#ifdef KS_FREERTOS_API

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

// Task Priorities
#define KS_COMPONENT_PRIORITY_HIGH      ( tskIDLE_PRIORITY + 3 )
#define KS_COMPONENT_PRIORITY_MEDIUM    ( tskIDLE_PRIORITY + 2 )
#define KS_COMPONENT_PRIORITY_LOW       ( tskIDLE_PRIORITY + 1 )

// Stack sizes
#define KS_COMPONENT_STACK_SIZE_XLARGE  ( configMINIMAL_STACK_SIZE * 4 )
#define KS_COMPONENT_STACK_SIZE_LARGE   ( configMINIMAL_STACK_SIZE * 3 )
#define KS_COMPONENT_STACK_SIZE_MEDIUM  ( configMINIMAL_STACK_SIZE * 2 )
#define KS_COMPONENT_STACK_SIZE_SMALL   ( configMINIMAL_STACK_SIZE * 1 )

#endif

// DRIVER TYPES
#ifdef KS_ASF_DRIVERS

#include "hal_gpio.h"
#include "hpl_gpio.h"
#include "hal_usart_os.h"
#include "hal_i2c_m_os.h"
#include "hal_spi_m_os.h"

typedef gpio_pull_mode KsGpioPullMode;
typedef gpio_direction KsGpioDirection;
typedef io_descriptor KsIoDescriptor;
typedef usart_os_descriptor KsUsartDescriptor;
typedef i2c_m_os_desc KsI2cDescriptor;
typedef spi_m_os_descriptor KsSpiDescriptor;

#elif defined(KS_POSIX_SIM)
typedef bool KsGpioPullMode;
typedef bool KsGpioDirection;
#endif

typedef uint16_t KsOpcode;

// NAMEOF MACROS
#include "nameof.hpp"

namespace kronos {
    // STD Pointers
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T>
    using Scope = std::unique_ptr<T>;

    // STD Containers
    using String = std::string;
    using StringView = std::string_view;
    using StringStream = std::stringstream;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Span = std::span<T>;
    template<typename K, typename V>
    using Map = std::unordered_map<K, V>;
    template<typename T>
    using Function = std::function<T>;
    template<typename T>
    using PriorityQueue = std::priority_queue<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Set = std::unordered_set<T>;

    template<typename T, typename ... Args>
    Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T, Args...>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T, Args...>(std::forward<Args>(args)...);
    }

    struct TypeInfo {
        KsIdType id;
        StringView name;
    };

    template<typename T>
    KsIdType ClassID() {
        return typeid(T).hash_code();
    }

    template<typename T>
    TypeInfo GetTypeInfo() {
        return {
            .id = ClassID<T>(),
            .name = NAMEOF_SHORT_TYPE(T)
        };
    }

}