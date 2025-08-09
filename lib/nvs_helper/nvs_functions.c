#include "nvs_functions.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include <stdint.h>
#include <string.h>

static const char *TAG = "nvs_params";

void load_param(const char*    key,   const char* key_array[],
                const uint16_t value, uint16_t*   to_param_array[],
                const int array_keys_count)
{
    for (int i = 0; i < array_keys_count; i++)
        if (strcmp(key, key_array[i]) == 0)
        {
            *to_param_array[i] = value;
            return;
        }
}

esp_err_t nvs_init(void)
{
    esp_err_t nvs_error = nvs_flash_init();
    if (nvs_error == ESP_ERR_NVS_NO_FREE_PAGES || nvs_error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvs_error = nvs_flash_init();
    }

    return nvs_error;
}

esp_err_t nvs_open_file(const char* namespace, nvs_handle_t* handle)
{
    esp_err_t nvs_error = nvs_open(namespace, NVS_READWRITE, handle);
    if (nvs_error != ESP_OK)
        ESP_LOGE(TAG, "Failed to open NVS handle: %s", esp_err_to_name(nvs_error));

    return nvs_error;
}

void nvs_close_file(nvs_handle_t handle)
{
    nvs_close(handle);
}
