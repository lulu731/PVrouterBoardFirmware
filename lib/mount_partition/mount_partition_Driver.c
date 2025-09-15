
#include "mount_partition_Hardware.h"
#include "mount_partition_Driver.h"

void mount_part_create(const char *partition_name, const char *mount_point)
{
    fs_init(partition_name, mount_point);
}

mount_error_t mount_part()
{
    hardware_mount_error_t error = fs_mount();
    if (error != HARD_MOUNT_OK)
    {
        return MOUNT_ERROR;
    }
    return MOUNT_OK;
}
