typedef uint16_t KsResult;
typedef uint16_t KsOpcode;

#define KS_OPCODE_INVALID           ((KsOpcode) 0xFFFF)
#define KS_OPCODE_EMPTY_QUEUE       ((KsOpcode) 0)
#define KS_OPCODE_SCHEDULER_TICK    ((KsOpcode) 1)

#define KS_SUCCESS                       ((KsResult) 0)

#define KS_ERROR_MISSING_COMPONENT     ((KsResult) 1)
#define KS_ERROR_MISSING_BUS             ((KsResult) 2)

#define KS_ERROR_DUPLICATE_COMPONENT     ((KsResult) 3)
#define KS_ERROR_DUPLICATE_BUS           ((KsResult) 4)
