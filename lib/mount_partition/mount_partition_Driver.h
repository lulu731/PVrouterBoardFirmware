
#ifndef MOUNT_PARTITION_DRIVER_H
#define MOUNT_PARTITION_DRIVER_H

#include <stdint.h>

typedef uint8_t mount_error_t;

#define MOUNT_OK    0
#define MOUNT_ERROR 1

void mount_part_create(const char* partition_name, const char* mount_point);
mount_error_t mount_part();

#endif // MOUNT_PARTITION_DRIVER_H
