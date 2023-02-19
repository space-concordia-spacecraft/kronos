#ifndef GS_PARAM_INTERNAL_RPARAM_H
#define GS_PARAM_INTERNAL_RPARAM_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#if (GS_PARAM_INTERNAL_USE)

#include <gs/param/rparam.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Max query payload in a single message (bytes).
*/
#define GS_RPARAM_QUERY_MAX_PAYLOAD	180

/**
   Macro for calculating total query message size, header + payload.
*/
#define RPARAM_QUERY_LENGTH(query, payload_size) (sizeof(*query) - sizeof(query->payload) + payload_size)

/**
   R(emote) parameter request codes.
*/
typedef enum {
    /**
       Get one or more parameters.
    */
    RPARAM_GET = 0x00,
    /**
       Reply to a request.
    */
    RPARAM_REPLY = 0x55,
    /**
       Set one or more parameters.
    */
    RPARAM_SET = 0xFF,
    // RPARAM_SET_TO_FILE = 0xEE,
    /**
       Download table specification.
    */
    RPARAM_TABLE = 0x44,
    /**
       Copy memory slot to memory slot.
       @version 4.x: Not supported.
    */
    RPARAM_COPY = 0x77,
    /**
       Load from file (slot) to memory (slot).
       @version 4.x: Only load from primary store - file (slot) is ignored.
    */
    RPARAM_LOAD = 0x88,
    /**
       Load from file (slot) to memory (slot).
       @version 4.x: load by name(s).
    */
    RPARAM_LOAD_FROM_STORE = 0x89,
    /**
       Save from memory (slot) to file (slot).
       @version 4.x: Only save to primary store - file (slot) is ignored.
    */
    RPARAM_SAVE = 0x99,
    /**
       Save from memory (slot) to file (slot).
       @version 4.x: save by name(s).
    */
    RPARAM_SAVE_TO_STORE = 0x9a,
    // RPARAM_CLEAR = 0xAA, - completely removed
} gs_rparam_action_t;

/**
   R(emote) parameter reply/completion codes.
*/
typedef enum {
    RPARAM_SET_OK = 1,
    RPARAM_LOAD_OK = 2,
    RPARAM_SAVE_OK = 3,
    RPARAM_COPY_OK = 4,
    // RPARAM_CLEAR_OK = 5,
    RPARAM_ERROR = 0xFF,
} gs_rparam_reply_t;

/**
   Payload - save/load to/from stores
   @version 4
*/
typedef struct __attribute__ ((packed)) {
    char table[25 + 1];
    char store[25 + 1];
    char slot[25 + 1];  
} gs_rparam_query_payload_store_t;

/**
   Payload.
*/
typedef union __attribute__ ((packed)) {
    uint16_t addr[0];        //! action = RPARAM_GET
    uint8_t packed[0];       //! action = RPARAM_REPLY | RPARAM_SET
    struct {                 //! action = RPARAM_COPY | RPARAM_LOAD | RPARM_SAVE
        uint8_t from;
        uint8_t to;
    } copy;
} gs_rparam_query_payload_t;
    
/**
   Protocol between client and server.
   @version 4.x: layout (size) has not changed - only naming of certain fields.
*/
typedef struct __attribute__ ((packed)) {
    /**
       Request (gs_rparam_action_t) or Reply (gs_rparam_reply_t).
    */
    uint8_t action;
    /**
       Table id.
       Name changed in 4.0 from \a mem.
    */
    uint8_t table_id;
    /**
       Length/size of \a payload in bytes.
    */
    uint16_t length;
    /**
       Fletcher's checksum.
    */
    uint16_t checksum;
    /**
       Sequence number when split over multiple frames (messages).
    */
    uint16_t seq;
    /**
       Total number of frames.
    */
    uint16_t total;
    /**
       Payload.
    */
    gs_rparam_query_payload_t payload;
} gs_rparam_query_t;

#ifdef __cplusplus
}
#endif
#endif
#endif
