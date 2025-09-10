#ifndef ESP_LITTLEFS_H
#define ESP_LITTLEFS_H

#include "esp_err.h"
#include <stdbool.h>

/**
 *Configuration structure for esp_vfs_littlefs_register.
 */
typedef struct {
    const char *base_path;            /**< Mounting point. */
    const char *partition_label;      /**< Label of partition to use. If partition_label, partition, and sdcard are all NULL,
                                           then the first partition with data subtype 'littlefs' will be used. */
//    const esp_partition_t* partition; /**< partition to use if partition_label is NULL */
    uint8_t format_if_mount_failed:1; /**< Format the file system if it fails to mount. */
    uint8_t read_only : 1;            /**< Mount the partition as read-only. */
    uint8_t dont_mount:1;             /**< Don't attempt to mount.*/
    uint8_t grow_on_mount:1;          /**< Grow filesystem to match partition size on mount.*/
} esp_vfs_littlefs_conf_t;

esp_err_t esp_vfs_littlefs_register(const esp_vfs_littlefs_conf_t * conf);

esp_err_t esp_vfs_littlefs_unregister(const char* partition_label);

#endif // ESP_LITTLEFS_H
