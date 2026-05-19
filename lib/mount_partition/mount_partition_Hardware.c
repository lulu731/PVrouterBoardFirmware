#include "mount_partition_Hardware.h"

#include "esp_littlefs.h"
#include "esp_log.h"

static const char *TAG = "mount_partition_hardware.c";

static esp_vfs_littlefs_conf_t esp_fs_config = {
    .format_if_mount_failed = false,
    .dont_mount =             false
};

/**
 * @brief Initializes the LittleFS filesystem with the given configuration.
 *
 * @param fs_config Configuration for the LittleFS filesystem.
 *
 * This function initializes the LittleFS filesystem with the given base path and partition label.
 */
void fs_init(const fs_config_t *fs_config)
{
    esp_fs_config.base_path =       fs_config->base_path;
    esp_fs_config.partition_label = fs_config->partition_label;
}

/**
 * @brief Mount a LittleFS partition
 *
 * @return HARD_MOUNT_OK if the mounting was successful, HARD_MOUNT_ERROR otherwise
 *
 * @details This function mounts a LittleFS partition based on the configuration set in fs_init.
 * If the mounting fails, it logs an error message with the reason for the failure.
 */
hardware_mount_error_t fs_mount()
{
    esp_err_t error = esp_vfs_littlefs_register(&esp_fs_config);

    if (error != ESP_OK) {
        if (error == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (error == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find LittleFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(error));
        }
        return HARD_MOUNT_ERROR;
    }

    return HARD_MOUNT_OK;
}