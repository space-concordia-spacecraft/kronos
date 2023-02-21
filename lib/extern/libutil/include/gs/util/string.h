#ifndef GS_UTIL_STRING_H
#define GS_UTIL_STRING_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   String utilitizes.

   All string parsing functions will return #GS_OK if the string was parsed entirely.
   If the string contains characters that are not part of the selected base, the functions will return #GS_ERROR_DATA.
   If the value parsed is bigger than the output type, the functions will return #GS_ERROR_OVERFLOW.
   Spaces are ignored by all functions.
*/

#include <gs/util/error.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Macro helper for concatening tokens.
*/
#define GS_STRINGZ(x)    #x

/**
   Stringify a preprocessing token.
*/
#define GS_DEF2STRING(x)   GS_STRINGZ(x)

/**
 * Strncpy (using size of destination) and forced zero termination.
 */
#define GS_STRNCPY(dst,src)   strncpy(dst,src,GS_ARRAY_SIZE(dst));dst[GS_ARRAY_SIZE(dst)-1] = 0

/**
   Convert string to int32 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_int32(const char * string, int32_t * value);

/**
   Convert string to uint32 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_uint32(const char * string, uint32_t * value);

/**
   Convert string to int64 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_int64(const char * string, int64_t * value);

/**
   Convert string to uint64 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_uint64(const char * string, uint64_t * value);

/**
   Convert string to int8 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 12 (decimal), 0x12 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_int8(const char * string, int8_t * value);

/**
   Convert string to uint8 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 12 (decimal), 0x12 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_uint8(const char * string, uint8_t * value);

/**
   Convert string to int16 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_int16(const char * string, int16_t * value);

/**
   Convert string to uint16 (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_uint16(const char * string, uint16_t * value);

/**
   Convert string to uint32 (hexadecimal).
   Accepts: hexadecimal (no leading 0x), e.g. a123, A123.
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_hex_to_uint32(const char * string, uint32_t * value);

/**
   Convert string to uint64 (hexadecimal).
   Accepts: hexadecimal (no leading 0x), e.g. a123, A123.
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_hex_to_uint64(const char * string, uint64_t * value);

/**
   Convert string to boolean.
   Accepts: true, false, on, off, 1, 0 (ignores case)
   @param[in] string string to convert.
   @param[out] pvalue converted value
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_bool(const char * string, bool * pvalue);

/**
   Convert string to float.
   @param[in] string string to convert.
   @param[out] pvalue converted value
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_float(const char * string, float * pvalue);

/**
   Convert string to double.
   @param[in] string string to convert.
   @param[out] pvalue converted value
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_double(const char * string, double * pvalue);
    
/**
   Return string for boolean value (true or false).
   @param[in] value value
   @return \a 'true' if input is true, else \a 'false'.
*/
const char * gs_string_from_bool(bool value);

/**
   Convert string to pointer (decimal or hexadecimal).
   Accepts: decimal or hexadecimal, 1234 (decimal), 0x1234 (hexadecimal)
   @param[in] string string to convert.
   @param[out] value converted value
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
gs_error_t gs_string_to_pointer(const char * string, void ** value);
    
/**
   Format size as Bytes, Kilo or Mega.

   Output examples: \a 512.0B, \a 1.0K and \a 1.0M.

   @param[in] size size in bytes
   @param[out] buffer formatted size
   @param[in] buffer_size size of \a buf
   @return GS_ERROR_OVERFLOW if the resulting value is larger than the output type
   @return GS_ERROR_DATA if the input string could not be parsed completely
   @return GS_ERROR_ARG if the input string is a NULL pointer
*/
char * gs_string_bytesize(long size, char *buffer, size_t buffer_size);

/**
   GS implementation of gcc's strtol
   Instead of setting errno this function takes a pointer to err which is set
   the same way as with gcc's strtol
   
   @param[in] nptr input string
   @param[out] endptr the pointer to the end of the string parsed
   @param[in] base number system (10 or 16)
   @param[out] err return value if overflow
   @return converted value
*/
int32_t gs_string_strto32int(const char *nptr, char **endptr, uint8_t base, gs_error_t * err);

/**
   GS implementation of gcc's strtoul
   Instead of setting errno this function takes a pointer to err which is set
   the same way as with gcc's strtoul
   
   @param[in] nptr input string
   @param[out] endptr the pointer to the end of the string parsed
   @param[in] base number system (10 or 16)
   @param[out] err return value if overflow
   @return converted value
*/
uint64_t gs_string_strto64uint(const char *nptr, char **endptr, uint8_t base, gs_error_t * err);

/**
   GS implementation of gcc's strtoul
   Instead of setting errno this function takes a pointer to err which is set
   the same way as with gcc's strtoul
   
   @param[in] nptr input string
   @param[out] endptr the pointer to the end of the string parsed
   @param[in] base number system (10 or 16)
   @param[out] err return value if overflow
   @return converted value
*/
int64_t gs_string_strto64int(const char *nptr, char **endptr, uint8_t base, gs_error_t * err);

/**
   GS implementation of gcc's strtoul
   Instead of setting errno this function takes a pointer to err which is set
   the same way as with gcc's strtoul
   
   @param[in] nptr input string
   @param[out] endptr the pointer to the end of the string parsed
   @param[in] base number system (10 or 16)
   @param[out] err return value if overflow
   @return converted value
*/
uint32_t gs_string_strto32uint(const char *nptr, char **endptr, uint8_t base, gs_error_t * err);

/**
   Returns pointer to first none-space character.

   @param[in] string string
   @return NULL if \a string is NULL, otherwise first none-space character.
*/
const char * gs_string_skip_leading_spaces(const char * string);

/**
   Check if a string is NULL or empty.

   @param[in] string string
   @return true if string is empty or NULL.
*/
bool gs_string_empty(const char * string);

/**
   Case-insentive wilcard match (similiar to fnmatch).

   Supports following wildcard(s):
   - * (asterix) zero or more characters.

   This may be extended in future versions and will not be considered a break of the API.

   @param[in] pattern pattern to match against \a string.
   @param[in] string string to match against \a pattern
   @return \a true if match, else \ false
*/
bool gs_string_match(const char * pattern, const char * string);

/**
   Returns \a true if string contains wildcards.

   @param[in] string string to check for wildcards.
   @return \a true if string contains wildcards recognized by gs_string_match().
*/
bool gs_string_has_wildcards(const char * string);

/**
   Trim string in buffer by removing leading/trailing white space.
  
   Uses isspace(c).

   @param[in] buffer buffer to trim.
   @param[in] buffer_size size of \a buffer.
*/
void gs_string_trim(char * buffer, size_t buffer_size);

/**
   Returns \a true if string ends with endswith.

   @param[in] string string to check
   @param[in] endswith string that string should end with
   @return \a true if string endswith endswith
*/
bool gs_string_endswith(const char * string, const char * endswith);

/**
   Extract suboption from a string.

   @param[in] options options string, e.g. \"/dev/ttyUSB1,speed=9600,parity=no,databits=8\". 
   @param[in] suboption sub-option to extract. If NULL or empty, the first option will be extracted (if present).
   @param[out] buf user buffer for returning value of sub-option.
   @param[in] buf_size size of \a buf user buffer.
   @return_gs_error_t
*/
gs_error_t gs_string_get_suboption(const char * options, const char * suboption, char * buf, size_t buf_size);

/**
   Extract suboption (as string) from a string.

   @param[in] options options string, e.g. \"/dev/ttyUSB1,speed=9600,parity=no,databits=8\". 
   @param[in] suboption sub-option to extract. If NULL or empty, the first option will be extracted (if present).
   @param[in] def default value, returned if sub-option isn't found.
   @param[out] buf user buffer for returning value of sub-option.
   @param[in] buf_size size of \a buf user buffer.
   @return If the sub-option isn't found, the \a def default value will be copied to \a buf and #GS_OK will be returned.
   @return_gs_error_t
*/
gs_error_t gs_string_get_suboption_string(const char * options, const char * suboption, const char * def, char * buf, size_t buf_size);

/**
   Extract suboption (as uint8) from a string.

   @param[in] options options string, e.g. \"/dev/ttyUSB1,speed=9600,parity=no,databits=8\". 
   @param[in] suboption sub-option to extract. If NULL or empty, the first option will be extracted (if present).
   @param[in] def default value, returned if sub-option isn't found.
   @param[out] value user supplied buffer for returning the value.
   @return If the sub-option isn't found, the \a def default value will be copied to \a value and #GS_OK will be returned.
   @return_gs_error_t
*/
gs_error_t gs_string_get_suboption_uint8(const char * options, const char * suboption, uint8_t def, uint8_t * value);

/**
   Extract suboption (as uint16) from a string.

   @param[in] options options string, e.g. \"/dev/ttyUSB1,speed=9600,parity=no,databits=8\". 
   @param[in] suboption sub-option to extract. If NULL or empty, the first option will be extracted (if present).
   @param[in] def default value, returned if sub-option isn't found.
   @param[out] value user supplied buffer for returning the value.
   @return If the sub-option isn't found, the \a def default value will be copied to \a value and #GS_OK will be returned.
   @return_gs_error_t
*/
gs_error_t gs_string_get_suboption_uint16(const char * options, const char * suboption, uint16_t def, uint16_t * value);

/**
   Extract suboption (as uint32) from a string.

   @param[in] options options string, e.g. \"/dev/ttyUSB1,speed=9600,parity=no,databits=8\".
   @param[in] suboption sub-option to extract. If NULL or empty, the first option will be extracted (if present).
   @param[in] def default value, returned if sub-option isn't found.
   @param[out] value user supplied buffer for returning the value.
   @return If the sub-option isn't found, the \a def default value will be copied to \a value and #GS_OK will be returned.
   @return_gs_error_t
*/
gs_error_t gs_string_get_suboption_uint32(const char * options, const char * suboption, uint32_t def, uint32_t * value);

/**
   Extract suboption (as bool) from a string.

   @param[in] options options string, e.g. \"/dev/ttyUSB1,speed=9600,parity=no,databits=8\". 
   @param[in] suboption sub-option to extract. If NULL or empty, the first option will be extracted (if present).
   @param[in] def default value, returned if sub-option isn't found.
   @param[out] value user supplied buffer for returning the value.
   @return If the sub-option isn't found, the \a def default value will be copied to \a value and #GS_OK will be returned.
   @return_gs_error_t
*/
gs_error_t gs_string_get_suboption_bool(const char * options, const char * suboption, bool def, bool * value);

/**
   Convience wrapping of standard snprintf.

   Implemented using standard vsnprintf().

   In the \a old days, it was possible to suppress return value from snprintf(), by using \a (void) cast. This is no longer supported in GCC 7,
   which makes it difficult to support older/newer GCC versions.
   This wrapper has the same functionality as standard snprintf, but eliminates the GCC check - it's still up to the programmer to take proper
   action in case of truncation or other errors.

   @param[in] buffer user allocated buffer for receiving formatted string.
   @param[in] buffer_size size of \a buffer
   @param[in] format printf syntax
   @return value from vsnprintf(). On success, number of characters printed (excluding the null byte used to end output to \a buffer).
*/
int gs_snprintf(char * buffer, size_t buffer_size, const char * format, ...) __attribute__ ((format (__printf__, 3, 4)));

#ifdef __cplusplus
}
#endif
#endif
