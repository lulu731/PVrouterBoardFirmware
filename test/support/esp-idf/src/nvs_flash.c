#include "nvs_flash.h"

#include "nvs.h"

extern int nvs_open_called;

// Control variables for test behavior
extern int nvs_flash_init_partition_error;
static int nvs_open_from_partition_error = 0;

esp_err_t nvs_flash_init_partition(const char *partition_label)
{
    if (nvs_flash_init_partition_error) {
        return ESP_ERR_NVS_NO_SPACE;
    }
    return ESP_OK;
}

esp_err_t nvs_open_from_partition(const char *partition_name,
                                  const char *namespace_name,
                                  nvs_open_mode_t open_mode,
                                  nvs_handle_t *out_handle)
{
    nvs_open_called++;
    if (nvs_open_from_partition_error) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    return ESP_OK;
}
