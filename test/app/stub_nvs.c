#include "nvs.h"
#include <stddef.h>

// Static storage for test values
static uint16_t stored_u16_value = 0;

esp_err_t nvs_entry_info(const nvs_iterator_t iterator, nvs_entry_info_t *out_info)
{
    return ESP_OK;
}

// Return error to indicate no entries found - this allows load_calibration_params
// to properly handle the "no calibration data" case without hanging
esp_err_t nvs_entry_find(const char *part_name, const char *namespace_name, nvs_type_t type, nvs_iterator_t *output_iterator)
{
    return ESP_ERR_INVALID_ARG;
}

esp_err_t nvs_entry_next(nvs_iterator_t *iterator)
{
    return ESP_OK;
}

void nvs_release_iterator(nvs_iterator_t iterator)
{
    return;
}

esp_err_t nvs_get_u16(nvs_handle_t handle, const char *key, uint16_t *out_value)
{
    if (out_value != NULL) {
        *out_value = stored_u16_value;
    }
    return ESP_OK;
}

esp_err_t nvs_set_u16(nvs_handle_t handle, const char *key, uint16_t value)
{
    stored_u16_value = value;
    return ESP_OK;
}

int nvs_open_called   = 0;
int nvs_commit_called = 0;
int nvs_close_called  = 0;

esp_err_t nvs_commit(nvs_handle_t handle)
{
    nvs_commit_called++;
    return ESP_OK;
}

esp_err_t nvs_open(const char* namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle)
{
    nvs_open_called++;
    return ESP_OK;
}

void nvs_close(nvs_handle_t handle)
{
    nvs_close_called++;
}
