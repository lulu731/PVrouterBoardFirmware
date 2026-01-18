
#include "mount_partition_Hardware.h"
#include "mount_partition_Driver.h"

/**
 * @brief Create a LittleFS partition
 *
 * @param[in] config Configuration for the partition
 *
 * @details This function initializes a LittleFS partition based on the provided configuration.
 */
void mount_part_create(const partition_config_t* config)
{
    const fs_config_t fs_config =
    {
        .base_path = config->base_path,
        .partition_label = config->partition_label
    };
    fs_init(&fs_config);
}

/**
 * @brief Mount a LittleFS partition
 *
 * @return HARD_MOUNT_OK if the mounting was successful, MOUNT_ERROR otherwise
 *
 * @details This function mounts a LittleFS partition based on the configuration set in mount_part_create.
 */
mount_error_t mount_part()
{
    hardware_mount_error_t error = fs_mount();
    if (error != HARD_MOUNT_OK)
    {
        return MOUNT_ERROR;
    }
    return MOUNT_OK;
}
