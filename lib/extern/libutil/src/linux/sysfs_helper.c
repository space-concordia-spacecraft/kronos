/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/linux/sysfs_helper.h>
#include <gs/util/log.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

gs_error_t gs_sysfs_write_file(const char *path, const char *value)
{
    log_trace("sysfs: write %s to %s", value, path);

    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        return GS_ERROR_HANDLE;
    }

    size_t len = strlen(value);
    ssize_t bytes = write(fd, value, len);
    close(fd);
    if (bytes < 0) {
        return GS_ERROR_NO_DATA;
    }

    return (len == (size_t)bytes) ? GS_OK : GS_ERROR_NO_DATA;
}

gs_error_t gs_sysfs_read_file(const char *path, char *value, size_t len)
{
    log_trace("sysfs: read %s", path);

    if (access(path, R_OK) != 0) {
        return GS_ERROR_ACCESS;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return GS_ERROR_HANDLE;
    }

    ssize_t bytes = read(fd, value, len);
    close(fd);
    if (bytes < 0) {
        return GS_ERROR_DATA;
    }

    // If possible we zero-terminate the string. When read() fills the buffer
    // completely we cannot do this without breaking the current API so that
    // corner case is left out for now.
    if ((size_t)bytes < len) {
        value[bytes] = '\0';
    }

    return GS_OK;
}
