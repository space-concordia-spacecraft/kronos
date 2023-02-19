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

#include <gs/util/base16.h>
#include <stdio.h>
#include <stdlib.h>

void base16_encode(const uint8_t * raw, size_t len, char *encoded)
{
        const uint8_t *raw_bytes = raw;
	char *encoded_bytes = encoded;
	size_t remaining = len;

	for (; remaining--; encoded_bytes += 2)
		snprintf(encoded_bytes, 3, "%02X", *(raw_bytes++));

}

int base16_decode(const char *encoded, uint8_t *raw)
{
    uint8_t *raw_bytes = raw;
    if (encoded) {
        const char *encoded_bytes = encoded;
        char buf[3];
        char *endp;

	while (encoded_bytes[0]) {
            if (!encoded_bytes[1]) {
                return GS_ERROR_ARG;
            }
            memcpy(buf, encoded_bytes, 2);
            buf[2] = '\0';
            *(raw_bytes++) = (uint8_t) strtoul(buf, &endp, 16);
            if (*endp != '\0') {
                return GS_ERROR_ARG;
            }
            encoded_bytes += 2;
	}
    }
    return (int)(raw_bytes - raw);
}
