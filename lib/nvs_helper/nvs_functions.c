#include "nvs_functions.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include <stdint.h>
#include <string.h>

static const char *TAG = "nvs_params";

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
