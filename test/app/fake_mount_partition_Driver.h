#ifndef MOUNT_PARTITION_DRIVER_H
#define MOUNT_PARTITION_DRIVER_H

#include <stdint.h>

typedef uint8_t mount_error_t;

#define MOUNT_OK    0
#define MOUNT_ERROR 1

typedef struct partition_config {
    const char* base_path;
    const char* partition_label;
} partition_config_t;

void mount_part_create(const partition_config_t* config);
mount_error_t mount_part();

#endif // MOUNT_PARTITION_DRIVER_H