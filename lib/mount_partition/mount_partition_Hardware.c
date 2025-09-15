#include "mount_partition_Hardware.h"

#include "esp_littlefs.h"
#include "esp_log.h"

static const char *TAG = "mount_partition_hardware.c";

static esp_vfs_littlefs_conf_t fs_config = {
    .format_if_mount_failed = false,
    .dont_mount = false
};

void fs_init(const char* partition_name, const char* mount_point)
{
    fs_config.base_path = mount_point;
    fs_config.partition_label = partition_name;
}

hardware_mount_error_t fs_mount()
{
    esp_err_t error = esp_vfs_littlefs_register(&fs_config);

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
