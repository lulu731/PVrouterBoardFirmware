#ifndef MOUNT_PARTITION_HARDWARE_H
#define MOUNT_PARTITION_HARDWARE_H

#include <stdint.h>
//#include <esp_littlefs.h>

typedef uint8_t hardware_mount_error_t;

#define HARD_MOUNT_OK    0
#define HARD_MOUNT_ERROR 1

typedef struct
{
    const char *base_path;            /**< Mounting point. */
    const char *partition_label;      /**< Label of partition to use.*/
} fs_config_t;

void fs_init(const char* partition_name, const char* mount_point);
hardware_mount_error_t fs_mount();

#endif // MOUNT_PARTITION_HARDWARE_H>
