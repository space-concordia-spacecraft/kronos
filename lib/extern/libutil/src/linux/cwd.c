/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <gs/util/unistd.h>
#include <unistd.h>
#include <errno.h>

gs_error_t gs_getcwd(char * buf, size_t bufsize)
{
    if (buf && bufsize) {
        char * wd = getcwd(buf, bufsize);
        if (wd) {
            return GS_OK;
        }
        switch(errno) {
            case ENAMETOOLONG:
            case ERANGE:
                return GS_ERROR_RANGE;

            case EACCES:
            case ENOENT:
                return GS_ERROR_NOT_FOUND;

            default:
                break;
        }
    }
    return GS_ERROR_ARG;
}
