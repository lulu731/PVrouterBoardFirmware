
#include "mount_partition_Hardware.h"
#include "mount_partition_Driver.h"

void mount_part_create(const partition_config_t* config)
{
    const fs_config_t fs_config =
    {
        .base_path = config->base_path,
        .partition_label = config->partition_label
    };
    fs_init(&fs_config);
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
