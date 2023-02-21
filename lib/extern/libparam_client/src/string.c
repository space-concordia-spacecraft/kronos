/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#define GS_PARAM_INTERNAL_USE 1

#include <gs/param/internal/types.h>
#include <gs/param/internal/table.h>
#include <gs/util/string.h>
#include <gs/util/log.h>
#include <gs/util/check.h>
#include <gs/util/bytebuffer.h>
#include <ctype.h>

size_t gs_param_calc_table_size(const gs_param_table_row_t * rows, size_t row_count)
{
    if (rows && row_count) {
        const gs_param_table_row_t * last_row = rows;
        // table rows may not be in assending address- so we have to run through the entire table.
        for (size_t i = 0; i < row_count; ++i, ++rows) {
            if (GS_PARAM_ADDR(rows) > GS_PARAM_ADDR(last_row)) {
                last_row = rows;
            }
        }
        return (GS_PARAM_ADDR(last_row) + (GS_PARAM_SIZE(last_row) * GS_PARAM_ARRAY_SIZE(last_row)));
    }
    return 0;
}

const gs_param_table_row_t * gs_param_row_by_name(const char * name, const gs_param_table_row_t * rows, size_t row_count)
{
    if (rows) {
        for (unsigned int i = 0; i < row_count; ++i, ++rows) {
            if (GS_PGM_STRNCASECMP(name, rows->name, GS_PARAM_MAX_NAME) == 0) {
                return rows;
            }
        }
    }
    return NULL;
}

const gs_param_table_row_t * gs_param_row_by_address(uint16_t addr, const gs_param_table_row_t * rows, size_t row_count)
{
    if (rows) {
        for (unsigned int i = 0; i < row_count; ++i, ++rows) {
            const unsigned int param_array_size = GS_PARAM_ARRAY_SIZE(rows);
            const unsigned int param_size = GS_PARAM_SIZE(rows);
            const unsigned int param_addr = GS_PARAM_ADDR(rows);
            for (unsigned int j = 0; j < param_array_size; ++j) {
                if ((param_addr + (j * param_size)) == addr) {
                    return rows;
                }
            }
        }
    }
    return NULL;
}

/**
   memcpy is used, because data/value may not be aligned correctly and cause crash if accessed directly.
*/
gs_error_t gs_param_to_string_buffer(const gs_param_table_row_t * param, const void * value, bool with_type, uint32_t flags, gs_bytebuffer_t *bb)
{
    flags |= param->flags;
    const uint8_t param_type = GS_PARAM_TYPE(param);
    switch (param_type) {
        case GS_PARAM_BOOL: {
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "BL  ");
            }
            gs_bytebuffer_printf(bb, "%s", *(uint8_t *) value ? "true" : "false");
            break;
        }
        case GS_PARAM_INT8: {
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "I8  ");
            }
            gs_bytebuffer_printf(bb, "%d", *(int8_t *) value);
            break;
        }
        case PARAM_X8:
            flags |= GS_PARAM_F_SHOW_HEX;
            // fallthrough
        case GS_PARAM_UINT8: {
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "U8  ");
            }
            if (flags & GS_PARAM_F_SHOW_HEX) {
                gs_bytebuffer_printf(bb, "0x%02"PRIx8, *(uint8_t *) value);
            } else {
                gs_bytebuffer_printf(bb, "%u", *(uint8_t *) value);
            }
            break;
        }
        case GS_PARAM_INT16: {
            int16_t tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "I16 ");
            }
            gs_bytebuffer_printf(bb, "%"PRId16, tmp);
            break;
        }
        case PARAM_X16:
            flags |= GS_PARAM_F_SHOW_HEX;
            // fallthrough
        case GS_PARAM_UINT16: {
            uint16_t tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "U16 ");
            }
            if (flags & GS_PARAM_F_SHOW_HEX) {
                gs_bytebuffer_printf(bb, "0x%04"PRIx16, tmp);
            } else {
                gs_bytebuffer_printf(bb, "%"PRIu16, tmp);
            }
            break;
        }
        case GS_PARAM_INT32: {
            int32_t tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "I32 ");
            }
            gs_bytebuffer_printf(bb, "%"PRId32, tmp);
            break;
        }
        case PARAM_X32:
            flags |= GS_PARAM_F_SHOW_HEX;
            // fallthrough
        case GS_PARAM_UINT32: {
            uint32_t tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "U32 ");
            }
            if (flags & GS_PARAM_F_SHOW_HEX) {
                gs_bytebuffer_printf(bb, "0x%08"PRIx32, tmp);
            } else {
                gs_bytebuffer_printf(bb, "%"PRIu32, tmp);
            }
            break;
        }
#ifdef PRIu64
        case GS_PARAM_INT64: {
            int64_t tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "I64 ");
            }
            gs_bytebuffer_printf(bb, "%"PRId64, tmp);
            break;
        }
        case PARAM_X64:
            flags |= GS_PARAM_F_SHOW_HEX;
            // fallthrough
        case GS_PARAM_UINT64: {
            uint64_t tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "U64 ");
            }
            if (flags & GS_PARAM_F_SHOW_HEX) {
                gs_bytebuffer_printf(bb, "0x%016"PRIx64, tmp);
            } else {
                gs_bytebuffer_printf(bb, "%"PRIu64, tmp);
            }
            break;
        }
#endif
        case GS_PARAM_FLOAT: {
            float tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "FLT ");
            }
            if (flags & GS_PARAM_F_SHOW_SCIENTIFIC) {
                gs_bytebuffer_printf(bb, "%e", (double) tmp);
            } else {
                gs_bytebuffer_printf(bb, "%f", (double) tmp);
            }
            break;
        }
        case GS_PARAM_DOUBLE: {
            double tmp;
            memcpy(&tmp, value, sizeof(tmp));
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "DBL ");
            }
            if (flags & GS_PARAM_F_SHOW_SCIENTIFIC) {
                gs_bytebuffer_printf(bb, "%e", tmp);
            } else {
                gs_bytebuffer_printf(bb, "%f", tmp);
            }
            break;
        }
        case GS_PARAM_STRING: {
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "STR ");
            }
            gs_bytebuffer_append(bb, "\"", 1);
            // handle missing NUL termination.
            const size_t len = strnlen((const char*)value, GS_PARAM_SIZE(param));
            gs_bytebuffer_append(bb, value, len);
            gs_bytebuffer_append(bb, "\"", 1);
            break;
        }
        case GS_PARAM_DATA: {
            if (with_type == 1) {
                gs_bytebuffer_printf(bb, "DAT ");
            }
            for (int i = 0; i < GS_PARAM_SIZE(param); i++) {
                gs_bytebuffer_printf(bb, "%02"PRIX8, ((uint8_t *) value)[i]);
            }
            break;
        }
        default: {
            log_error("%s: Unknown param type %u", __FUNCTION__, param_type);
            break;
        }
    }

    return GS_OK;
}

gs_error_t gs_param_to_string2(const gs_param_table_row_t * param, const void * value, bool with_type, uint32_t flags, char * buf, unsigned int buf_size, unsigned int buf_pos, unsigned int * buf_written)
{
    GS_CHECK_ARG(buf_pos <= buf_size);
    gs_bytebuffer_t bb;
    gs_bytebuffer_init(&bb, &buf[buf_pos], (buf_size - buf_pos));
    gs_error_t error = gs_param_to_string_buffer(param, value, with_type, flags, &bb);
    if (error == GS_OK) {
        gs_bytebuffer_get_as_string(&bb, &error); // this will add NUL termination, but may truncate buffer
        error = gs_bytebuffer_get_state(&bb);
        if (buf_written) {
            *buf_written = bb.used;
        }
    }
    return error;
}

const char * gs_param_type_to_string(gs_param_type_t type)
{
    switch (type) {
	case GS_PARAM_BOOL:     return "bool";
	case GS_PARAM_UINT8:    return "uint8_t";
	case GS_PARAM_UINT16:   return "uint16_t";
	case GS_PARAM_UINT32:   return "uint32_t";
	case GS_PARAM_UINT64:   return "uint65_t";
	case GS_PARAM_INT8:     return "int8_t";
	case GS_PARAM_INT16:    return "int16_t";
	case GS_PARAM_INT32:    return "int32_t";
	case GS_PARAM_INT64:    return "int64_t";
	case PARAM_X8:          return "uint8_t";
	case PARAM_X16:         return "uint16_t";
	case PARAM_X32:         return "uint32_t";
	case PARAM_X64:         return "uint64_t";
	case GS_PARAM_FLOAT:    return "float";
	case GS_PARAM_DOUBLE:   return "double";
	case GS_PARAM_STRING:   return "char";
	case GS_PARAM_DATA:     return "char";
    }
    return "";
}

static inline int to_int(char c)
{
    if (c >= '0' && c <= '9') return      c - '0';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    return -1;
}

gs_error_t gs_param_from_string(const gs_param_table_row_t * param, const char * string, void * value)
{
    if ((param == NULL) || (string == NULL) || (value == NULL)) {
        return GS_ERROR_ARG;
    }

    if (GS_PARAM_TYPE(param) != GS_PARAM_STRING) {
        // skip only space -  not white-space, e.g. isspace()
        for (; *string == ' '; ++string);
    }

    gs_error_t error = GS_OK;

    switch(GS_PARAM_TYPE(param)) {

        case GS_PARAM_BOOL:
            {
                bool parsein = false;
                error = gs_string_to_bool(string, &parsein);
                if (error == GS_OK) {
                    *((uint8_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_UINT8:
            {
                uint8_t parsein;
                error = gs_string_to_uint8(string, &parsein);
                if (error == GS_OK) {
                    *((uint8_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_UINT16:
            {
                uint16_t parsein;
                error = gs_string_to_uint16(string, &parsein);
                if (error == GS_OK) {
                    *((uint16_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_UINT32:
            {
                uint32_t parsein;
                error = gs_string_to_uint32(string, &parsein);
                if (error == GS_OK) {
                    *((uint32_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_UINT64:
            {
                uint64_t parsein;
                error = gs_string_to_uint64(string, &parsein);
                if (error == GS_OK) {
                    *((uint64_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_INT8:
            {
                int8_t parsein;
                error = gs_string_to_int8(string, &parsein);
                if (error == GS_OK) {
                    *((int8_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_INT16:
            {
                int16_t parsein;
                error = gs_string_to_int16(string, &parsein);
                if (error == GS_OK) {
                    *((int16_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_INT32:
            {
                int32_t parsein;
                error = gs_string_to_int32(string, &parsein);
                if (error == GS_OK) {
                    *((int32_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_INT64:
            {
                int64_t parsein;
                error = gs_string_to_int64(string, &parsein);
                if (error == GS_OK) {
                    *((int64_t *) value) = parsein;
                }
            }
            break;

        case PARAM_X8:
            {
                uint32_t parsein;
                error = gs_string_hex_to_uint32(string, &parsein);
                if (error == GS_OK) {
                    if (parsein <= UINT8_MAX) {
                        *((uint8_t *) value) = parsein;
                    } else {
                        error = GS_ERROR_OVERFLOW;
                    }
                }
            }
            break;

        case PARAM_X16:
            {
                uint32_t parsein;
                error = gs_string_hex_to_uint32(string, &parsein);
                if (error == GS_OK) {
                    if (parsein <= UINT16_MAX) {
                        *((uint16_t *) value) = parsein;
                    } else {
                        error = GS_ERROR_OVERFLOW;
                    }
                }
            }
            break;

        case PARAM_X32:
            {
                uint32_t parsein;
                error = gs_string_hex_to_uint32(string, &parsein);
                if (error == GS_OK) {
                    *((uint32_t *) value) = parsein;
                }
            }
            break;

        case PARAM_X64:
            {
                uint64_t parsein;
                error = gs_string_hex_to_uint64(string, &parsein);
                if (error == GS_OK) {
                    *((uint64_t *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_FLOAT:
            {
                float parsein;
                error = gs_string_to_float(string, &parsein);
                if (error == GS_OK) {
                    *((float *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_DOUBLE:
            {
                double parsein;
                error = gs_string_to_double(string, &parsein);
                if (error == GS_OK) {
                    *((double *) value) = parsein;
                }
            }
            break;

        case GS_PARAM_STRING:
            {
                const size_t ilen = strnlen(string, GS_PARAM_SIZE(param) + 1); // 0 terminator
                if (ilen <= GS_PARAM_SIZE(param)) {
                    memset(value, 0, GS_PARAM_SIZE(param));
                    memcpy(value, string, ilen);
                } else {
                    error = GS_ERROR_OVERFLOW;
                }
            }
            break;

        case GS_PARAM_DATA:
            {
                const size_t MAX_LEN = (GS_PARAM_SIZE(param) * 2);
                const size_t ilen = strnlen(string, MAX_LEN + 1);
                if (ilen > MAX_LEN) {
                    error = GS_ERROR_OVERFLOW;
                } else if ((ilen % 2) == 0) {
                    
                    // validate data first - not to end up with invalid/strange data
                    for (unsigned int i = 0; i < ilen; ++i) {
                        if (to_int(string[i]) < 0) {
                            error = GS_ERROR_DATA;
                            break;
                        }
                    }
                    if (error == GS_OK) {
                        uint8_t * out = (uint8_t *) value;
                        memset(out, 0, GS_PARAM_SIZE(param));
                        for (unsigned int i = 0; i < ilen; i += 2, ++out) {
                            *out = (16 * to_int(string[i])) + to_int(string[i+1]);
                        }
                        error = GS_OK;
                    }
                } else {
                    error = GS_ERROR_DATA;
                }
            }
            break;
    }

    return error;
}

gs_error_t gs_param_list_single_to_stream(gs_param_table_instance_t * tinst, const gs_param_table_row_t * param,
                                          bool list_data, uint32_t flags, FILE * out)
{
    if (param == NULL) {
        return GS_ERROR_HANDLE;
    }

    gs_error_t error = GS_OK;
    const uint16_t addr = GS_PARAM_ADDR(param);

    fprintf(out, "  0x%04X %-16.14"GS_PGM_FMT_STR, addr, param->name); // ensure missing NUL termination doesn't cause problems.

    if (list_data) {
        const gs_param_type_t param_type = GS_PARAM_TYPE(param);
        const unsigned int param_array_size = GS_PARAM_ARRAY_SIZE(param);
        const unsigned int param_size = GS_PARAM_SIZE(param);
        uint8_t value[param_size];
        char buf[100];
        for (unsigned int j = 0; (j < param_array_size) && (error == GS_OK); j++) {
            error = gs_param_get(tinst, addr + (param_size * j), param_type, value, param_size, 0);
            if (error == GS_OK) {
                gs_param_to_string2(param, value, (j == 0) ? 1 : 0, flags, buf, sizeof(buf), 0, NULL);
                fprintf(out, "%s ", buf);
            }
        }
    }
    fprintf(out, "\r\n");
    return error;
}

gs_error_t gs_param_list_to_stream(gs_param_table_instance_t * tinst, bool list_data, uint32_t flags, FILE * out)
{
    GS_CHECK_HANDLE(tinst != NULL);

    gs_error_t error = GS_OK;
    for (unsigned int i = 0; (i < tinst->row_count) && (error == GS_OK); ++i) {
        error = gs_param_list_single_to_stream(tinst, &tinst->rows[i], list_data, flags, out);
    }

    return error;
}

gs_error_t gs_param_parse_name_and_array_index(const char * inp, char * name, size_t size_name, uint8_t * return_index, bool * return_is_array)
{
    if (inp == NULL) {
        return GS_ERROR_ARG;
    }

    uint8_t a_index;
    size_t name_len;
    gs_error_t error;
    bool is_array;
    const char * pai = strchr(inp, '['); // look for array index
    if (pai) {
        name_len = pai - inp;
        char tmp[20];
        GS_STRNCPY(tmp, pai+1);
        char * endp = strchr(tmp, ']');
        if (endp) {
            *endp = 0;
        }
        error = gs_string_to_uint8(tmp, &a_index);
        is_array = true;
    } else {
        error = GS_OK;
        name_len = strlen(inp);
        is_array = false;
        a_index = 0;
    }

    if (error == GS_OK) {
        if (name_len >= size_name) {
            error = GS_ERROR_OVERFLOW;
        } else {
            strncpy(name, inp, name_len);
            name[name_len] = 0;

            // remove trailing white-space
            if (name_len) {
                for (int i = name_len-1; i >= 0; --i) {
                    if (name[i] && isspace((int)name[i])) {
                        name[i] = 0;
                    } else {
                        break;
                    }
                }
            }

            if (return_index) {
                *return_index = (is_array) ? a_index : 0;
            }
            if (return_is_array) {
                *return_is_array = is_array;
            }
        }
    }

    return error;
}
