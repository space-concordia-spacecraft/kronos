set(RELIANCE_EDGE_SRC
        # RELIANCE EDGE
        "bdev/bdev.c"
        "config/redconf.c"
        "core/driver/blockio.c"
        "core/driver/buffer.c"
        "core/driver/buffercmn.c"
        "core/driver/core.c"
        "core/driver/dir.c"
        "core/driver/format.c"
        "core/driver/imap.c"
        "core/driver/imapextern.c"
        "core/driver/imapinline.c"
        "core/driver/inode.c"
        "core/driver/inodedata.c"
        "core/driver/volume.c"
        "fse/fse.c"
        "os/freertos/services/osassert.c"
        "os/freertos/services/osbdev.c"
        "os/freertos/services/osclock.c"
        "os/freertos/services/osmutex.c"
        "os/freertos/services/osoutput.c"
        "os/freertos/services/ostask.c"
        "os/freertos/services/ostimestamp.c"
        "posix/path.c"
        "posix/posix.c"
        "util/bitmap.c"
        "util/crc.c"
        "util/endian.c"
        "util/heap.c"
        "util/memory.c"
        "util/namelen.c"
        "util/sign.c"
        "util/string.c")

set(RELIANCE_EDGE_INCLUDE_DIRS
        "include"
        "config"
        "core/driver"
        "core/include"
        "os/freertos/include"
        "os/freertos/services")

add_mcu_library("reliance_edge" ${RELIANCE_EDGE_SRC})
target_include_directories("reliance_edge" PUBLIC ${RELIANCE_EDGE_INCLUDE_DIRS})
target_link_libraries("reliance_edge" PUBLIC asf)