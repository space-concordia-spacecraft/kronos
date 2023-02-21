/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/string.h>
#include <gs/util/minmax.h>
#include <gs/util/check.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#if (__AVR__ == 0)
#include <math.h>
#endif

#ifndef GS_STRING_GET_SUBOPTION_UNIT_TEST
#define GS_STRING_GET_SUBOPTION_UNIT_TEST 0
#endif

const char * gs_string_skip_leading_spaces(const char * string)
{
    if (string) {
        for (; *string == ' '; ++string);
    }
    return string;
}

gs_error_t gs_string_to_int32(const char * string, int32_t * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (!(isdigit((int)string[0]) || (string[0] == '-'))) {
        return GS_ERROR_DATA;
    }

    int32_t tmp;
    uint8_t base = 10;

    // check for hexadecimal notation
    if ((string[0] == '0') && ((string[1] == 'x') || (string[1] == 'X')))
    {
        base = 16;
    }

    const char *desired_end = string + strlen(string);
    // The desired end should point to that last non-space char in the string
    while ((isspace((int)desired_end[0]) || (desired_end[0] == '\0')) && (desired_end > string))
    {
        desired_end--;
    }
    char *end;
    gs_error_t err = GS_OK;
    tmp = gs_string_strto32int(string, &end, base, &err);
    if (err != GS_OK)
    {
        return err;
    }
    if (desired_end != end-1)
    {
        return GS_ERROR_DATA;
    }

    if (return_value)
    {
        *return_value = tmp;
    }

    return GS_OK;
}

gs_error_t gs_string_to_int8(const char * string, int8_t * return_value)
{
    int32_t value;
    gs_error_t error = gs_string_to_int32(string, &value);
    if (error == GS_OK) {
        if ((value >= INT8_MIN) && (value <= INT8_MAX)) {
            if (return_value) {
                *return_value = (int8_t) value;
            }
        } else {
            error = GS_ERROR_OVERFLOW;
        }
    }
    return error;
}

gs_error_t gs_string_to_uint8(const char * string, uint8_t * return_value)
{
    uint32_t value;
    gs_error_t error = gs_string_to_uint32(string, &value);
    if (error == GS_OK) {
        if (value <= UINT8_MAX) {
            if (return_value) {
                *return_value = (uint8_t) value;
            }
        } else {
            error = GS_ERROR_OVERFLOW;
        }
    }
    return error;
}

gs_error_t gs_string_to_int16(const char * string, int16_t * return_value)
{
    int32_t value;
    gs_error_t error = gs_string_to_int32(string, &value);
    if (error == GS_OK) {
        if ((value >= INT16_MIN) && (value <= INT16_MAX)) {
            if (return_value) {
                *return_value = (int16_t) value;
            }
        } else {
            error = GS_ERROR_OVERFLOW;
        }
    }
    return error;
}

gs_error_t gs_string_to_uint16(const char * string, uint16_t * return_value)
{
    uint32_t value;
    gs_error_t error = gs_string_to_uint32(string, &value);
    if (error == GS_OK) {
        if (value <= UINT16_MAX) {
            if (return_value) {
                *return_value = (uint16_t) value;
            }
        } else {
            error = GS_ERROR_OVERFLOW;
        }
    }
    return error;
}

gs_error_t gs_string_to_uint32(const char * string, uint32_t * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (!isdigit((int)string[0])) {
        return GS_ERROR_DATA;
    }

    uint32_t tmp;
    uint8_t base = 10;

    // check for hexadecimal notation
    if ((string[0] == '0') && ((string[1] == 'x') || (string[1] == 'X')))
    {
        base = 16;
    }
    const char *desired_end = string + strlen(string);
    // The desired end should point to that last non-space char in the string
    while ((isspace((int)desired_end[0]) || desired_end[0] == 0) && (desired_end > string))
    {
        desired_end--;
    }
    char *end;
    gs_error_t err = GS_OK;
    tmp = gs_string_strto32uint(string, &end, base, &err);

    if (err != GS_OK)
    {
        return err;
    }
    if (desired_end != end-1)
    {
        return GS_ERROR_DATA;
    }


    if (return_value) {
        *return_value = tmp;
    }

    return GS_OK;
}

gs_error_t gs_string_to_uint64(const char * string, uint64_t * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (!isdigit((int)string[0]))
    {
        return GS_ERROR_DATA;
    }

    uint64_t tmp;
    uint8_t base = 10;

    // check for hexadecimal notation
    if ((string[0] == '0') && ((string[1] == 'x') || (string[1] == 'X'))) {
        base = 16;
    }
    const char *desired_end = string + strlen(string);
    // The desired end should point to that last non-space char in the string
    while ((isspace((int)desired_end[0]) || desired_end[0] == 0) && (desired_end > string))
    {
        desired_end--;
    }
    char *end;
    gs_error_t err = GS_OK;
    tmp = gs_string_strto64uint(string, &end, base, &err);
    if (err != GS_OK)
    {
        return err;
    }

    if (desired_end != end-1)
    {
        return GS_ERROR_DATA;
    }
    

    if (return_value) {
        *return_value = tmp;
    }

    return GS_OK;
}

gs_error_t gs_string_to_int64(const char * string, int64_t * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (!(isdigit((int)string[0]) || (string[0] == '-')))
    {
        return GS_ERROR_DATA;
    }

    int64_t tmp;
    uint8_t base = 10;

    // check for hexadecimal notation
    if ((string[0] == '0') && ((string[1] == 'x') || (string[1] == 'X'))) {
        base = 16;
    }
    const char *desired_end = string + strlen(string);
    // The desired end should point to that last non-space char in the string
    while ((isspace((int)desired_end[0]) || desired_end[0] == 0) && (desired_end > string))
    {
        desired_end--;
    }
    char *end;
    gs_error_t err = GS_OK;
    tmp = gs_string_strto64int(string, &end, base, &err);
    if (err != GS_OK)
    {
        return err;
    }

    if (desired_end != end-1)
    {
        return GS_ERROR_DATA;
    }
    

    if (return_value) {
        *return_value = tmp;
    }

    return GS_OK;
}

gs_error_t gs_string_hex_to_uint32(const char * string, uint32_t * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (!isxdigit((int)string[0])) {
        return GS_ERROR_DATA;
    }

    const char *desired_end = string + strlen(string);
    // The desired end should point to that last non-space char in the string
    while ((isspace((int)desired_end[0]) || desired_end[0] == 0) && (desired_end > string))
    {
        desired_end--;
    }
    char *end;
    gs_error_t err = GS_OK;
    uint32_t tmp = gs_string_strto32uint(string, &end, 16, &err);

    if (err != GS_OK)
    {
        return err;
    }
    if (desired_end != end-1)
    {
        return GS_ERROR_DATA;
    }

    if (return_value) {
        *return_value = tmp;
    }

    return GS_OK;
}

gs_error_t gs_string_hex_to_uint64(const char * string, uint64_t * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (!isxdigit((int)string[0]))
    {
        return GS_ERROR_DATA;
    }

    const char *desired_end = string + strlen(string);
    // The desired end should point to that last non-space char in the string
    while ((isspace((int)desired_end[0]) || desired_end[0] == 0) && (desired_end > string))
    {
        desired_end--;
    }
    char *end;
    gs_error_t err = GS_OK;
    uint64_t tmp = gs_string_strto64uint(string, &end, 16, &err);
    if (err != GS_OK)
    {
        return err;
    }

    if (desired_end != end-1)
    {
        return GS_ERROR_DATA;
    }

    if (return_value) {
        *return_value = tmp;
    }

    return GS_OK;
}

#if (__AVR__ == 0)
gs_error_t gs_string_to_float(const char * string, float * pvalue)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (gs_string_empty(string)) {
        return GS_ERROR_DATA;
    }

    // float strtof(const char *nptr, char **endptr);
    char * endp = NULL;
    float tmp = strtof(string, &endp);
    //if ((endp == NULL) || (gs_string_empty(gs_string_skip_leading_spaces(endp)) == false) || (string == endp)) {
    if ((endp == NULL) || (gs_string_empty(gs_string_skip_leading_spaces(endp)) == false) || (string == endp) || isinf(tmp)) {
        return GS_ERROR_DATA;
    }

    if (pvalue) {
        *pvalue = tmp;
    }

    return GS_OK;
}

gs_error_t gs_string_to_double(const char * string, double * pvalue)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }

    if (gs_string_empty(string)) {
        return GS_ERROR_DATA;
    }

    // double strtod(const char *nptr, char **endptr);
    char * endp = NULL;
    double tmp = strtod(string, &endp);
    if ((endp == NULL) || (gs_string_empty(gs_string_skip_leading_spaces(endp)) == false) || isinf(tmp)) {
        return GS_ERROR_DATA;
    }

    if (pvalue) {
        *pvalue = tmp;
    }

    return GS_OK;
}
#endif

#define GS_STRING_BOOL_TRUE  "true"
#define GS_STRING_BOOL_FALSE "false"

const char * gs_string_from_bool(bool value)
{
    if (value) {
        return GS_STRING_BOOL_TRUE;
    } else {
        return GS_STRING_BOOL_FALSE;
    }
}

gs_error_t gs_string_to_bool(const char * string, bool * return_value)
{
    string = gs_string_skip_leading_spaces(string);
    if (string == NULL) {
        return GS_ERROR_ARG;
    }
    if (string[0] == 0) {
        return GS_ERROR_DATA;
    }

    bool value = false;
    if (strcasecmp(string, GS_STRING_BOOL_TRUE) == 0) {
        value = true;
    } else if (strcasecmp(string, GS_STRING_BOOL_FALSE) == 0) {
        value = false;
    } else if (strcasecmp(string, "on") == 0) {
        value = true;
    } else if (strcasecmp(string, "off") == 0) {
        value = false;
    } else if (strcasecmp(string, "1") == 0) {
        value = true;
    } else if (strcasecmp(string, "0") == 0) {
        value = false;
    } else if (strcasecmp(string, "yes") == 0) {
        value = true;
    } else if (strcasecmp(string, "no") == 0) {
        value = false;
    } else {
        return GS_ERROR_DATA;
    }

    if (return_value) {
        *return_value = value;
    }

    return GS_OK;
}

char * gs_string_bytesize(long n, char *buf, size_t buf_size)
{
    char postfix = 'B';
    double size = (double) n;
    if (n >= 1048576) {
        size /= 1048576.0;
        postfix = 'M';
    } else if (n >= 1024) {
        size /= 1024.0;
        postfix = 'K';
    }
    snprintf(buf, buf_size, "%.1f%c", size, postfix);
    return buf;
}

gs_error_t gs_string_to_pointer(const char * string, void ** value)
{
#if __LP64__
    uint64_t tmp;
    gs_error_t error = gs_string_to_uint64(string, &tmp);
    if ((error == GS_OK) && value) {
        *value = GS_TYPES_UINT2PTR(tmp);
    }
    return error;
#else
    uint32_t tmp;
    gs_error_t error = gs_string_to_uint32(string, &tmp);
    if ((error == GS_OK) && value) {
        *value = GS_TYPES_UINT2PTR(tmp);
    }
    return error;
#endif
}

bool gs_string_empty(const char * string)
{
    if ((string == NULL) || (string[0] == 0)) {
        return true;
    }
    return false;
}

bool gs_string_match(const char * pattern, const char * string)
{
    if (string && pattern) {
        const char * last_wildcard = NULL;
        while (*string || *pattern) {
            int p = tolower((int)*pattern);
            if (*pattern == '*') {
                last_wildcard = pattern;
                ++pattern;
                p = tolower((int)*pattern);
                for (; *string && (tolower((int)*string) != p); ++string);
            }

            int s = tolower((int)*string);
            if (s != p) {
                if (s && last_wildcard) {
                    // reset pattern match and try again
                    pattern = last_wildcard;
                    continue;
                }
                return false;
            }

            if (s) {
                ++string;
            }
            if (p) {
                ++pattern;
            }
        }
        if ((*string == 0) && (*pattern == 0)) {
            return true;
        }
    }
    return false;
}

bool gs_string_has_wildcards(const char * string)
{
    if (strchr(string, '*')) {
        return true;
    }
    // future wildcard
    //if (strchr(str, '?')) {
    //    return true;
    //}
    return false;
}    

void gs_string_trim(char * buffer, size_t buffer_size)
{
    // remove trailing stuff
    int len = strnlen(buffer, buffer_size);
    if (len) {
        for (int i = (len - 1); i >= 0; --i) {
            if (isspace((int)buffer[i])) {
                buffer[i] = 0;
            } else {
                break;
            }
        }
    }

    char * start;
    for (start = buffer; *start && isspace((int)*start); ++start);
    if (*start && (start != buffer)) {
        // move chars up
        for (; *start; ++start) {
            *buffer++ = *start;
        }
        *buffer = 0;
    }
}

bool gs_string_endswith(const char * string, const char * endswith)
{
    if (string == NULL || endswith == NULL) {
        return false;
    }

    int str_len = strlen(string);
    int endswith_len = strlen(endswith);

    return (str_len >= endswith_len) &&
            (0 == strcmp(string + (str_len-endswith_len), endswith));
}

static size_t suboption_len(const char * ref, const char * end)
{
    if (ref) {
        size_t len = (end) ? ((size_t)(end - ref)) : strlen(ref);
        for (; len && (ref[len - 1] == ' '); --len);
        return len;
    }
    return 0;
}

static gs_error_t suboption_copy(const char * data, size_t len, char * buf, size_t buf_size, gs_error_t error)
{
    if (len >= buf_size) {
        error = GS_ERROR_OVERFLOW;
        len = (buf_size - 1);
    }

    if (data == NULL) {
        len = 0;
    } else {
        strncpy(buf, data, len);
    }

    buf[len] = 0;

    return error;
}

gs_error_t gs_string_get_suboption(const char * options, const char * suboption, char * buf, size_t buf_size)
{
    GS_CHECK_ARG(options != NULL);
    GS_CHECK_ARG((buf != NULL) && (buf_size > 0));

    const char * next = options;
    for (;next;) {
        const char * key = next;
        if (*key == ',') {
            key = NULL; // no key-value
        }
        next = strchr(next, ',');

        const char * value = NULL;
        if (key) {
            for (; *key == ' '; ++key);

            value = strchr(key, '=');
            if (value == NULL) {
                // no value
            } else if (next && (value >= next)) {
                // no value
                value = NULL;
            }
        }

        const unsigned int key_len = suboption_len(key, value ? value : next);

        if (value) {
            if (*value == '=') {
                ++value;
            }
            for (; *value == ' '; ++value);
        }

        const unsigned int value_len = suboption_len(value, next);

        if (GS_STRING_GET_SUBOPTION_UNIT_TEST) { // -> #define
            printf(" key=[%.*s], len=%u, value=[%.*s], len=%u, next=[%s]\n",
                   key_len, key ? key : "", key_len,
                   value_len, value ? value : "", value_len,
                   next ? next : "");
        }

        // if suboption is empty, it means get value of first element - ignoring any key
        if (gs_string_empty(suboption)) {
            if (value) {
                return suboption_copy(value, value_len, buf, buf_size, GS_OK);
            }
            if (key) {
                return suboption_copy(key, key_len, buf, buf_size, GS_OK);
            }
            return suboption_copy(NULL, 0, buf, buf_size, GS_OK); // empty
        }

        if ((key_len == strlen(suboption)) && (strncasecmp(key, suboption, key_len) == 0)) {
            return suboption_copy(value, value_len, buf, buf_size, GS_OK);
        }

        if (next) {
            ++next;
            if (next[0] == 0) {
                next = NULL;
            }
        }
    }

    // not found - return default
    return suboption_copy(NULL, 0, buf, buf_size, GS_ERROR_NOT_FOUND);
}

static const char * _suboption_name(const char * suboption)
{
    if (gs_string_empty(suboption)) {
        return "first suboption";
    }
    return suboption;
}

#define _get_suboption(_type)                                           \
    char buf[20];                                                       \
    gs_error_t error = gs_string_get_suboption(options, suboption, buf, sizeof(buf)); \
    if (error == GS_OK) {                                               \
        error = gs_string_to_##_type(buf, value);                        \
    }                                                                   \
    if (error) {                                                        \
        if (error == GS_ERROR_NOT_FOUND) {                              \
            error = GS_OK;                                              \
        } else {                                                        \
            log_error("Failed to extract suboption [%s] from [%s], error: %d", _suboption_name(suboption), options, error); \
        }                                                               \
        *value = def;                                                   \
    }                                                                   \
    return error;                                                       \

gs_error_t gs_string_get_suboption_uint8(const char * options, const char * suboption, uint8_t def, uint8_t * value)
{
    _get_suboption(uint8)
}

gs_error_t gs_string_get_suboption_uint16(const char * options, const char * suboption, uint16_t def, uint16_t * value)
{
    _get_suboption(uint16)
}

gs_error_t gs_string_get_suboption_uint32(const char * options, const char * suboption, uint32_t def, uint32_t * value)
{
    _get_suboption(uint32)
}

gs_error_t gs_string_get_suboption_string(const char * options, const char * suboption, const char * def, char * buf, size_t buf_size)
{
    gs_error_t error = gs_string_get_suboption(options, suboption, buf, buf_size);
    if (error) {
        if (error == GS_ERROR_NOT_FOUND) {
            error = GS_OK;
        }
        error = suboption_copy(def, def ? strlen(def) : 0, buf, buf_size, error);
    }
    return error;
}

gs_error_t gs_string_get_suboption_bool(const char * options, const char * suboption, bool def, bool * value)
{
    char buf[20];
    gs_error_t error = gs_string_get_suboption(options, suboption, buf, sizeof(buf));
    if (error == GS_OK) {
        if (gs_string_empty(buf) || (suboption && (strcasecmp(suboption, buf) == 0))) {
            // this means 'true', a=21,active,a=22
            *value = true;
        } else {
            error = gs_string_to_bool(buf, value);
        }
    }
    if (error) {
        if (error == GS_ERROR_NOT_FOUND) {
            error = GS_OK;
        } else {
            log_error("Failed to extract suboption [%s] from [%s], error: %d", _suboption_name(suboption), options, error);
        }
        *value = def;
    }
    return error;
}

int gs_snprintf(char * buffer, size_t buffer_size, const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    int res = vsnprintf(buffer, buffer_size, format, ap);
    va_end(ap);
    return res;
}
