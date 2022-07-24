#pragma once

#include "asf.h"

//! \typedef Type that tells you the result of
typedef uint16_t KsResult;
typedef void* KsCmdResult;
typedef uint16_t KsEventCode;
typedef uint16_t KsOpcode;

//! \def Event code for an invalid event
#define KS_EVENT_CODE_INVALID                   ((KsEventCode)  0xFFFF)

//! \def Event code
#define KS_EVENT_CODE_EMPTY_QUEUE               ((KsEventCode)  0)
#define KS_EVENT_CODE_TIMER_TICK                ((KsEventCode)  1)
#define KS_EVENT_CODE_RATE_GROUP_TICK           ((KsEventCode)  2)
#define KS_EVENT_CODE_HEALTH_PING               ((KsEventCode)  3)
#define KS_EVENT_CODE_HEALTH_RESPONSE           ((KsEventCode)  4)
#define KS_EVENT_CODE_LOG_MESSAGE               ((KsEventCode)  5)
#define KS_EVENT_CODE_OPEN_FILE                 ((KsEventCode)  6)
#define KS_EVENT_CODE_DISPATCH_COMMAND          ((KsEventCode)  7)
#define KS_EVENT_CODE_READ                      ((KsEventCode)  8)
#define KS_EVENT_CODE_WRITE                     ((KsEventCode)  9)
#define KS_EVENT_CODE_FILE_READ                 ((KsEventCode) 10)
#define KS_EVENT_CODE_FILE_WRITE                ((KsEventCode) 11)
#define KS_EVENT_CODE_FILE_UPLOAD               ((KsEventCode) 12)
#define KS_EVENT_CODE_FILE_DOWNLOAD             ((KsEventCode) 13)

#define KS_CMDRESULT_NORETURN               ((KsCmdResult) nullptr)

#define KS_SUCCESS                          ((KsResult)  0)
#define KS_ERROR                            ((KsResult)  1)
#define KS_ERROR_MISSING_COMPONENT          ((KsResult)  2)
#define KS_ERROR_MISSING_BUS                ((KsResult)  3)
#define KS_ERROR_DUPLICATE_COMPONENT        ((KsResult)  4)
#define KS_ERROR_DUPLICATE_BUS              ((KsResult)  5)
#define KS_ERROR_FILE_NOT_OPEN              ((KsResult)  6)
#define KS_ERROR_FILE_UNABLE_TO_OPEN        ((KsResult)  7)
#define KS_ERROR_FILE_UNABLE_TO_CLOSE       ((KsResult)  8)
#define KS_ERROR_FILE_UNABLE_TO_WRITE       ((KsResult)  9)
#define KS_ERROR_FILE_UNABLE_TO_READ        ((KsResult) 10)
#define KS_ERROR_FILE_UNABLE_TO_RENAME      ((KsResult) 11)
#define KS_ERROR_FILE_UNABLE_TO_MOVE        ((KsResult) 12)
#define KS_ERROR_FILE_UNABLE_TO_GET_STATUS  ((KsResult) 13)
#define KS_ERROR_FILE_UNABLE_TO_INIT        ((KsResult) 14)
#define KS_ERROR_FILE_SYNC_FAILED           ((KsResult) 15)
#define KS_ERROR_APOLLO_HEADER              ((KsResult) 16)
#define KS_ERROR_APOLLO_VERSION             ((KsResult) 17)
