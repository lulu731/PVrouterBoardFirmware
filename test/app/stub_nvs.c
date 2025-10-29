#include "nvs.h"

esp_err_t nvs_entry_info(const nvs_iterator_t iterator, nvs_entry_info_t *out_info)
{
    return ESP_OK;
}

// return error to pass app test
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
    return ESP_OK;
}