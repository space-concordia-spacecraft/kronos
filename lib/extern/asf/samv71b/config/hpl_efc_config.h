/* Auto-generated config file hpl_efc_config.h */
#ifndef HPL_EFC_CONFIG_H
#define HPL_EFC_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <o> Page size
// <512=> 512 bytes
// <id> conf_flash_page_size
#ifndef CONF_FLASH_PAGE_SIZE
#define CONF_FLASH_PAGE_SIZE 512
#endif

// <o> Erasing area/block size
// <8192=> 8K bytes
// <id> conf_flash_erasing_blk_size
#ifndef CONF_FLASH_ERASING_BLK_SIZE
#define CONF_FLASH_ERASING_BLK_SIZE 8192
#endif

// <e> Enable re-write or unalign operations
// <id> conf_flash_rewr_unalign_op
#ifndef CONF_FLASH_REWR_UNALIGN_OP
#define CONF_FLASH_REWR_UNALIGN_OP 1
#endif

// <o> Where pages are cached while doing erasing
// <0x0=> STATIC
// <0x1=> STACK
// <i> STATIC: the buffer is a static buffer used by the driver.
// <i> STACK: the buffer is in C stack.
// <id> conf_flash_op_buf_loc
#ifndef CONF_FLASH_OP_BUF_LOC
#define CONF_FLASH_OP_BUF_LOC 0
#endif

// <q> Enable re-write (overwrite) of falsh data with other area unchanged
// <i> If it's not enabled, the target area should be erased before write
// <id> conf_flash_rewrite_enable
#ifndef CONF_FLASH_REWRITE_ENABLE
#define CONF_FLASH_REWRITE_ENABLE 1
#endif

// <q> Unaligned append of falsh data
// <i> If it's not enabled, the target area should be append page by page
// <id> conf_flash_unalign_append_enable
#ifndef CONF_FLASH_UNALIGN_APPEND_ENABLE
#define CONF_FLASH_UNALIGN_APPEND_ENABLE 1
#endif

// <q> Enable unalign erase of flash with other area in erasing block unchanged
// <i> If it's not enabled, the target address must be aligned with erasing blocks
// <id> conf_flash_unalign_erase_enable
#ifndef CONF_FLASH_UNALIGN_ERASE_ENABLE
#define CONF_FLASH_UNALIGN_ERASE_ENABLE 1
#endif

// </e>

// <<< end of configuration section >>>

#endif // HPL_EFC_CONFIG_H
