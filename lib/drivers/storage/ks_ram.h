#pragma once

#include "lfs.h"

namespace kronos {
    constexpr size_t RAM_BUFFER_SIZE = 32 * 1024; // 32KB
    constexpr size_t BLOCK_SIZE = 512;
    constexpr size_t BLOCK_COUNT = RAM_BUFFER_SIZE / BLOCK_SIZE;

    uint8_t ramBuffer[RAM_BUFFER_SIZE] = {0};

    /**
     *
     * @param c
     * @param block
     * @param off
     * @param buffer
     * @param size
     * @return
     */
    int ram_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
        memcpy(buffer, &ramBuffer[block * c->block_size + off], size);
        return 0;
    }

    /**
     *
     * @param c
     * @param block
     * @param off
     * @param buffer
     * @param size
     * @return
     */
    int ram_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
        memcpy(&ramBuffer[block * c->block_size + off], buffer, size);
        return 0;
    }

    /**
     *
     * @param c
     * @param block
     * @return
     */
    int ram_erase(const struct lfs_config *c, lfs_block_t block) {
        memset(&ramBuffer[block * c->block_size], 0, c->block_size);
        return 0;
    }

    /**
     *
     * @param c
     * @return
     */
    int ram_sync(const struct lfs_config *c) {
        // Nothing to do for RAM-backed storage
        return 0;
    }

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
        .cache_size = 16,
        .lookahead_size = 16,
        .block_cycles = 500,
    };
}
