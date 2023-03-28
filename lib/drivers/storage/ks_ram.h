#pragma once

#include "lfs.h"

namespace kronos {
    KsResult setup();

    /**
     *
     * @param c
     * @param block
     * @param off
     * @param buffer
     * @param size
     * @return
     */
    int ram_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

    /**
     *
     * @param c
     * @param block
     * @param off
     * @param buffer
     * @param size
     * @return
     */
    int ram_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

    /**
     *
     * @param c
     * @param block
     * @return
     */
    int ram_erase(const struct lfs_config *c, lfs_block_t block);

    /**
     *
     * @param c
     * @return
     */
    int ram_sync(const struct lfs_config *c);

    // configuration of the filesystem is provided by this struct
    const struct lfs_config cfg = {
        // block device operations
        .read  = ram_read,
        .prog  = ram_prog,
        .erase = ram_erase,
        .sync  = ram_sync,

        // block device configuration
        .read_size = 16,
        .prog_size = 16,
        .block_size = 4096,
        .block_count = 128,
        .block_cycles = 500,
        .cache_size = 16,
        .lookahead_size = 16,
    };
}
