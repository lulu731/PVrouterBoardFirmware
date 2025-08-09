#include "nvs_params.h"
#include "nvs_functions.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

static const char *TAG = "nvs_params";

static const char* namespace = "meter_config";

extern const char* string_keys_array[];
extern uint16_t* keys_array[];
extern int keys_count;

void load_nvs_params(nvs_handle_t handle)
{
    nvs_iterator_t iterator = NULL;
    esp_err_t nvs_error = nvs_entry_find("nvs", namespace, NVS_TYPE_U16, &iterator);
    while (nvs_error == ESP_OK) {
        nvs_entry_info_t entry_info;
        nvs_entry_info(iterator, &entry_info);

        uint16_t param_value;
        nvs_get_u16(handle, entry_info.key, &param_value);

        load_param(entry_info.key, string_keys_array, param_value, keys_array, keys_count);

        nvs_error = nvs_entry_next(&iterator);
    }
    nvs_release_iterator(iterator);
}


void save_nvs_param(const char* key, const uint16_t value)
{
    // Open NVS handle
    ESP_LOGI(TAG, "\nOpening Non-Volatile Storage (NVS) handle...");
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(namespace, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return;
    }

    // Set value
    ESP_LOGI(TAG, "\nSetting value for key '%s'...", key);
    err = nvs_set_u16(nvs_handle, key, value);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) setting value for key '%s'!", esp_err_to_name(err), key);
        return;
    }

    // Commit changes
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    ESP_LOGI(TAG, "\nCommitting updates in NVS...");
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to commit NVS changes!");
    }

    // Close
    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "NVS handle closed.");

    ESP_LOGI(TAG, "Returned to app_main");
}