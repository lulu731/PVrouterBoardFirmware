#include "nvs.h"
#include <stddef.h>
#include <string.h>

// Static storage for test values
static uint16_t stored_u16_value = 0;

// Control variables for test behavior
int nvs_flash_init_partition_error = 0;
static int nvs_open_from_partition_error = 0;

// Iterator control for nvs_driver tests
static int iterator_position = 0;
static int max_iterator_position = 0;

// Error injection controls
static int nvs_entry_find_in_handle_error = 0;  // 0 = ESP_OK, 1 = ESP_ERR_INVALID_ARG
static int nvs_entry_next_error = 0;  // 0 = ESP_OK, 1 = ESP_ERR_NVS_NOT_FOUND, 2 = ESP_ERR_INVALID_ARG

// Include common_datas.h for test data
#include "common_datas.h"

void stub_nvs_set_flash_init_error(int error)
{
    nvs_flash_init_partition_error = error;
}

void stub_nvs_set_open_error(int error)
{
    nvs_open_from_partition_error = error;
}

void stub_nvs_reset_iterator(void)
{
    iterator_position = 0;
}

void stub_nvs_set_iterator_end(void)
{
    iterator_position = sizeof(nvs_datas) / sizeof(nvs_datas[0]);
}

void stub_nvs_set_entry_find_error(int error)
{
    nvs_entry_find_in_handle_error = error;
}

void stub_nvs_set_entry_next_error(int error)
{
    nvs_entry_next_error = error;
}

esp_err_t nvs_entry_info(const nvs_iterator_t iterator, nvs_entry_info_t *out_info)
{
    if (iterator_position >= (int)(sizeof(nvs_datas) / sizeof(nvs_datas[0]))) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    if (out_info != NULL) {
        strcpy(out_info->namespace_name, "meter_config");
        out_info->type = NVS_TYPE_U16;
        strcpy(out_info->key, nvs_datas[iterator_position].key);
    }
    return ESP_OK;
}

esp_err_t nvs_entry_find_in_handle(nvs_handle_t handle,
        nvs_type_t type,
        nvs_iterator_t *output_iterator)
{
    if (output_iterator == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    iterator_position = 0;
    if (sizeof(nvs_datas) / sizeof(nvs_datas[0]) == 0) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    *output_iterator = (nvs_iterator_t)1;
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
    if (iterator == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    iterator_position++;
    if (iterator_position >= (int)(sizeof(nvs_datas) / sizeof(nvs_datas[0]))) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    return ESP_OK;
}

void nvs_release_iterator(nvs_iterator_t iterator)
{
    iterator_position = 0;
}

esp_err_t nvs_get_u16(nvs_handle_t handle, const char *key, uint16_t *out_value)
{
    if (out_value != NULL) {
        // Use the current iterator position to get the value
        if (iterator_position < (int)(sizeof(nvs_datas) / sizeof(nvs_datas[0]))) {
            *out_value = nvs_datas[iterator_position].value;
        } else {
            *out_value = stored_u16_value;
        }
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
