#pragma once

#include "asf.h"

typedef uint16_t KsResult;
typedef void* KsCmdResult;
typedef uint16_t KsOpcode;

#define KS_OPCODE_INVALID                   ((KsOpcode) 0xFFFF)
#define KS_OPCODE_EMPTY_QUEUE               ((KsOpcode) 0)
#define KS_OPCODE_TIMER_TICK                ((KsOpcode) 1)
#define KS_OPCODE_RATE_GROUP_TICK           ((KsOpcode) 2)
#define KS_OPCODE_HEALTH_PING               ((KsOpcode) 3)
#define KS_OPCODE_HEALTH_RESPONSE           ((KsOpcode) 4)
#define KS_OPCODE_LOG_MESSAGE               ((KsOpcode) 5)
#define KS_OPCODE_OPEN_FILE                 ((KsOpcode) 6)

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
#define KS_ERROR_FILE_UNABLE_TO_GET_STATUS  ((KsResult) 11)