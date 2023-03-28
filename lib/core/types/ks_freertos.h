#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"

// Task Priorities
#define KS_COMPONENT_PRIORITY_HIGH      ( tskIDLE_PRIORITY + 3 )
#define KS_COMPONENT_PRIORITY_MEDIUM    ( tskIDLE_PRIORITY + 2 )
#define KS_COMPONENT_PRIORITY_LOW       ( tskIDLE_PRIORITY + 1 )

// Stack sizes
#define KS_COMPONENT_STACK_SIZE_XLARGE  ( configMINIMAL_STACK_SIZE * 4 )
#define KS_COMPONENT_STACK_SIZE_LARGE   ( configMINIMAL_STACK_SIZE * 3 )
#define KS_COMPONENT_STACK_SIZE_MEDIUM  ( configMINIMAL_STACK_SIZE * 2 )
#define KS_COMPONENT_STACK_SIZE_SMALL   ( configMINIMAL_STACK_SIZE * 1 )