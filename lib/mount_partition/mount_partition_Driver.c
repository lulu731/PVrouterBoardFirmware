
#include "mount_partition_Hardware.h"
#include "mount_partition_Driver.h"

void mount_part_create(const char *partition_name, const char *mount_point)
{
    init_fs(partition_name, mount_point);
}

mount_error_t mount_part()
{
    hardware_mount_error_t error = mount_fs();
    if (error != HARD_MOUNT_OK)
    {
        return MOUNT_ERROR;
    }
    return MOUNT_OK;
}
