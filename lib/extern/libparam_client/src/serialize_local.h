#ifndef SRC_SERIALIZE_LOCAL_H
#define SRC_SERIALIZE_LOCAL_H
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/param/internal/serialize.h>

gs_error_t gs_param_serialize_item_direct(gs_param_type_t param_type, unsigned int param_size, uint16_t addr, const void * item, uint32_t flags, uint8_t * buf, unsigned int buf_size, unsigned int * buf_pos);

#endif
