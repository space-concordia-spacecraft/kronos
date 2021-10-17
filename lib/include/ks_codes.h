#pragma once

#include "asf.h"

typedef uint16_t KsResult;
typedef void* KsCmdResult;
typedef uint16_t KsEventCode;
typedef uint16_t KsOpcode;

#define KS_EVENT_CODE_INVALID                   ((KsEventCode) 0xFFFF)
#define KS_EVENT_CODE_EMPTY_QUEUE               ((KsEventCode) 0)
#define KS_EVENT_CODE_TIMER_TICK                ((KsEventCode) 1)
#define KS_EVENT_CODE_RATE_GROUP_TICK           ((KsEventCode) 2)
#define KS_EVENT_CODE_HEALTH_PING               ((KsEventCode) 3)
#define KS_EVENT_CODE_HEALTH_RESPONSE           ((KsEventCode) 4)
#define KS_EVENT_CODE_LOG_MESSAGE               ((KsEventCode) 5)
#define KS_EVENT_CODE_OPEN_FILE                 ((KsEventCode) 6)
#define KS_EVENT_CODE_DISPATCH_COMMAND          ((KsEventCode) 7)
#define KS_EVENT_CODE_READ                      ((KsEventCode) 8)
#define KS_EVENT_CODE_WRITE                     ((KsEventCode) 9)

#define KS_CMDRESULT_NORETURN               ((KsCmdResult) nullptr)

#define KS_SUCCESS                          ((KsResult) 0)
#define KS_ERROR_MISSING_COMPONENT          ((KsResult) 1)
#define KS_ERROR_MISSING_BUS                ((KsResult) 2)
#define KS_ERROR_DUPLICATE_COMPONENT        ((KsResult) 3)
#define KS_ERROR_DUPLICATE_BUS              ((KsResult) 4)
#define KS_ERROR_FILE_NOT_OPEN              ((KsResult) 5)
#define KS_ERROR_FILE_UNABLE_TO_OPEN        ((KsResult) 6)
#define KS_ERROR_FILE_UNABLE_TO_CLOSE       ((KsResult) 7)
#define KS_ERROR_FILE_UNABLE_TO_WRITE       ((KsResult) 8)
#define KS_ERROR_FILE_UNABLE_TO_READ        ((KsResult) 9)
#define KS_ERROR_FILE_UNABLE_TO_RENAME      ((KsResult) 10)
#define KS_ERROR_FILE_UNABLE_TO_MOVE        ((KsResult) 11)
#define KS_ERROR_FILE_UNABLE_TO_GET_STATUS  ((KsResult) 12)