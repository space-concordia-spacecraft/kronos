/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/csp/error.h>

gs_error_t gs_csp_error(int csp_error)
{
    switch (csp_error) {
        case CSP_ERR_NONE: /* No error */
            return GS_OK;

        case CSP_ERR_NOMEM: /* Not enough memory */
            return GS_ERROR_ALLOC;

        case CSP_ERR_INVAL: /* Invalid argument */
            return GS_ERROR_ARG;

        case CSP_ERR_TIMEDOUT: /* Operation timed out */
            return GS_ERROR_TIMEOUT;

        case CSP_ERR_USED: /* Resource already in use */
            return GS_ERROR_IN_USE;

        case CSP_ERR_NOTSUP: /* Operation not supported */
            return GS_ERROR_NOT_SUPPORTED;

        case CSP_ERR_BUSY: /* Device or resource busy */
            return GS_ERROR_BUSY;

        case CSP_ERR_ALREADY: /* Connection already in progress */
            return GS_ERROR_ALREADY_IN_PROGRESS;

        case CSP_ERR_RESET: /* Connection reset */
            return GS_ERROR_CONNECTION_RESET;

        case CSP_ERR_NOBUFS: /* No more buffer space available */
            return GS_ERROR_NO_BUFFERS;

        case CSP_ERR_TX: /* Transmission failed */
        case CSP_ERR_DRIVER: /* Error in driver layer */
            return GS_ERROR_IO;

        case CSP_ERR_AGAIN:
            return GS_ERROR_AGAIN;

        case CSP_ERR_HMAC: /* HMAC failed */
        case CSP_ERR_XTEA: /* XTEA failed */
        case CSP_ERR_CRC32: /* CRC32 failed */
            return GS_ERROR_DATA;

        default:
            break;
    }
    return csp_error;
}
