#include "fake_mount_partition_Driver.h"

mount_error_t fake_mount_result = MOUNT_OK;

void mount_part_create(const partition_config_t* config)
{
    return;
}

mount_error_t mount_part()
{
    return fake_mount_result;
}
