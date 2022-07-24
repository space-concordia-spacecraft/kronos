// Stub printf and scanf
#include "std_stub.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Microchip ASF
#include "board.h"
#include "asf.h"

// Types
#include "ks_file.h"
#include "ks_queue.h"

// Component Base Classes
#include "ks_component_base.h"
#include "ks_component_passive.h"
#include "ks_component_queued.h"
#include "ks_component_active.h"

// Framework
#include "ks_codes.h"
#include "ks_bus.h"
#include "ks_framework.h"

// Components
#include "ks_file_manager.h"
#include "ks_file_transfer.h"
#include "ks_health_monitor.h"
#include "ks_logger.h"
#include "ks_telemetry_logger.h"
#include "ks_rate_group.h"
#include "ks_timer.h"
#include "ks_uart.h"
#include "ks_communications.h"

// Profiler
#include "ks_profiler.h"

#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>

void console_init();