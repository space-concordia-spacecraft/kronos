#ifndef GS_UTIL_BASE16_H
#define GS_UTIL_BASE16_H
/*
 * Copyright (C) 2010 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/**
   @file

   Encoding and decoding base16 arrays to and from strings.
*/

#include <gs/util/error.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Calculate length of base16-encoded data

   @param raw_len Raw data length
   @return Encoded string length (excluding NUL)
*/
static inline size_t base16_encoded_len(size_t raw_len)
{
    return (2 * raw_len);
}

/**
   Calculate maximum length of base16-decoded string
   @param encoded Encoded string
   @return Maximum length of raw data
*/
static inline size_t base16_decoded_max_len(const char *encoded)
{
    return ((strlen(encoded) + 1) / 2);
}

/**
   Base16-encode data

   The buffer must be the correct length for the encoded string.  Use
   something like
 
      char buf[ base16_encoded_len ( len ) + 1 ];
 
   (the +1 is for the terminating NUL) to provide a buffer of the
   correct size.

   @param raw Raw data
   @param len Length of raw data
   @param encoded Buffer for encoded string
*/
void base16_encode(const uint8_t *raw, size_t len, char *encoded);

/**
   Base16-decode data

   The buffer must be large enough to contain the decoded data.  Use
   something like

       char buf[ base16_decoded_max_len ( encoded ) ];

   to provide a buffer of the correct size.

   @param encoded Encoded string
   @param raw Raw data
   @return Length of raw data, or negative error (gs_error_t)
*/
int base16_decode(const char *encoded, uint8_t *raw);

#ifdef __cplusplus
}
#endif
#endif
