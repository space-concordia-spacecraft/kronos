// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Microchip ASF
#include "board.h"
#include "asf.h"

// Types
//#include "ks_file.h"
#include "ks_hashmap.h"
#include "ks_iterable.h"
#include "ks_queue.h"
#include "ks_string.h"
#include "ks_vector.h"

// Framework
#include "ks_opcodes.h"
#include "ks_bus.h"
#include "ks_framework.h"

// Components
//#include "ks_file_manager.h"
//#include "ks_health_monitor.h"
#include "ks_logger.h"
#include "ks_timer.h"

void console_init();