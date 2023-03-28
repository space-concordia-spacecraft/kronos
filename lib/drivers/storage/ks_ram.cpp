#include "ks_ram.h"

namespace kronos {
    static constexpr size_t RAM_BUFFER_SIZE = 32 * 1024; // 32KB
    static constexpr size_t BLOCK_SIZE = 512;
    static constexpr size_t BLOCK_COUNT = RAM_BUFFER_SIZE / BLOCK_SIZE;

    static uint8_t ramBuffer[RAM_BUFFER_SIZE] = {0};

    static SemaphoreHandle_t ram_mutex;

    KsResult setup() {
        ram_mutex = xSemaphoreCreateMutex();
        if (ram_mutex == nullptr) KS_THROW(ks_error);

        return ks_success;
    }

    int ram_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
        xSemaphoreTake(ram_mutex, portMAX_DELAY);
        memcpy(buffer, &ramBuffer[block * c->block_size + off], size);
        xSemaphoreGive(ram_mutex);
        return ks_success;
    }

    int ram_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
        xSemaphoreTake(ram_mutex, portMAX_DELAY);
        memcpy(&ramBuffer[block * c->block_size + off], buffer, size);
        xSemaphoreGive(ram_mutex);
        return ks_success;
    }

    int ram_erase(const struct lfs_config *c, lfs_block_t block) {
        xSemaphoreTake(ram_mutex, portMAX_DELAY);
        memset(&ramBuffer[block * c->block_size], 0, c->block_size);
        xSemaphoreGive(ram_mutex);
        return ks_success;
    }

    int ram_sync(const struct lfs_config *c) {
        // Nothing to do for RAM-backed storage
        return ks_success;
    }

}
