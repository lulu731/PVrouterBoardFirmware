#ifndef MOUNT_PARTITION_HARDWARE_H
#define MOUNT_PARTITION_HARDWARE_H

#include <stdint.h>

typedef uint8_t hardware_mount_error_t;

#define HARD_MOUNT_OK    0
#define HARD_MOUNT_ERROR 1

typedef struct fs_config
{
    const char *base_path;            /**< Mounting point. */
    const char *partition_label;      /**< Label of partition to use.*/
} fs_config_t;

void fs_init(const fs_config_t* fs_config);
hardware_mount_error_t fs_mount();

#endif // MOUNT_PARTITION_HARDWARE_H>
