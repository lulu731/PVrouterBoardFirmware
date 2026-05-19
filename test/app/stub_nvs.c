#include "nvs.h"
#include <stddef.h>
#include <string.h>

// Static storage for test values
static uint16_t stored_u16_value = 0;
static char stored_key[32] = {0};

// Control variables for test behavior
static int nvs_flash_init_partition_error = 0;
static int nvs_open_from_partition_error = 0;

// Functions to control stub error states
void stub_nvs_set_flash_init_error(int error)
{
    nvs_flash_init_partition_error = error;
}

void stub_nvs_set_open_error(int error)
{
    nvs_open_from_partition_error = error;
}

// Global variables to control stub behavior for tests
int nvs_entry_find_in_handle_return_ESP_OK = 0;
int nvs_entry_next_return_ESP_OK = 0;
int nvs_entry_info_should_fill_key = 0;

esp_err_t nvs_entry_info(const nvs_iterator_t iterator, nvs_entry_info_t *out_info)
{
    if (out_info != NULL && nvs_entry_info_should_fill_key) {
        strcpy(out_info->namespace_name, "meter_config");
        out_info->type = NVS_TYPE_U16;
        strcpy(out_info->key, stored_key);
    }
    return ESP_OK;
}

esp_err_t nvs_entry_find_in_handle(nvs_handle_t handle,
        nvs_type_t type,
        nvs_iterator_t *output_iterator)
{
    if (nvs_entry_find_in_handle_return_ESP_OK) {
        return ESP_OK;
    }
    return ESP_ERR_NVS_NOT_FOUND;
}

// Return error to indicate no entries found - this allows load_calibration_params
// to properly handle the "no calibration data" case without hanging
esp_err_t nvs_entry_find(const char *part_name, const char *namespace_name, nvs_type_t type, nvs_iterator_t *output_iterator)
{
    if (nvs_entry_find_in_handle_return_ESP_OK) {
        return ESP_OK;
    }
    return ESP_ERR_INVALID_ARG;
}

esp_err_t nvs_entry_next(nvs_iterator_t *iterator)
{
    if (nvs_entry_next_return_ESP_OK) {
        return ESP_OK;
    }
    return ESP_ERR_NVS_NOT_FOUND;
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

// Stub implementations for nvs_storage functions
#include "nvs_flash.h"

esp_err_t nvs_flash_init_partition(const char *partition_label)
{
    return ESP_OK;
}

esp_err_t nvs_open_from_partition(const char *partition_name, const char *namespace_name, nvs_open_mode_t open_mode, nvs_handle_t *out_handle)
{
    nvs_open_called++;
    return ESP_OK;
}
