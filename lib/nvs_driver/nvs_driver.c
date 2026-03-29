#include "nvs_driver.h"

#include "nvs.h"
#include "nvs_flash.h"

#include <stddef.h>

static const char* namespace = "meter_config";

nvs_handle_t handle = 0;
static nvs_iterator_t iterator = NULL;

/**
 * @brief Initializes the NVS driver.
 *
 * This function initializes the NVS driver by calling @ref nvs_flash_init.
 * If the initialization fails, it returns @ref NVS_INIT_ERROR. Otherwise, it returns @ref NVS_OK.
 *
 * @return NVS_OK if the initialization succeeds, NVS_INIT_ERROR if it fails.
 */
nvs_err_t nvs_init(void)
{
    if (nvs_flash_init() != ESP_OK)
    {
        return NVS_INIT_ERROR;
    }
    return NVS_OK;
};

/**
 * @brief Returns the first NVS data entry.
 *
 * This function returns the first NVS data entry found in the NVS partition.
 * If the entry is not found, it returns an NVS data entry with key set to NULL and value set to 0.
 *
 * @return The first NVS data entry found in the NVS partition if the entry is found, an NVS data entry with key set to NULL and value set to 0 if the entry is not found.
 */
nvs_data_t get_first_nvs_data(void)
{
    nvs_data_t nvs_data =
    {
        .key = NULL,
        .value = 0
    };

    esp_err_t nvs_error = nvs_entry_find("nvs", namespace, NVS_TYPE_U16, &iterator);
    switch (nvs_error)
    {
        case ESP_OK:
            nvs_entry_info_t entry_info;
            nvs_entry_info(iterator, &entry_info);

            uint16_t param_value;
            nvs_get_u16(handle, entry_info.key, &param_value);

            nvs_data.key = entry_info.key;
            nvs_data.value = param_value;

            break;

        case ESP_ERR_INVALID_ARG:
            break;

        default:
            nvs_release_iterator(iterator);
            break;
    }
    return nvs_data;
}

/**
 * @brief Returns the next NVS data entry.
 *
 * This function returns the next NVS data entry found in the NVS partition.
 * If the entry is not found, it releases the iterator and returns an NVS data entry with key set to NULL and value set to 0.
 *
 * @return The next NVS data entry found in the NVS partition if the entry is found, an NVS data entry with key set to NULL and value set to 0 if the entry is not found.
 */
nvs_data_t get_next_nvs_data(void)
{
    nvs_data_t nvs_data =
    {
        .key = NULL,
        .value = 0
    };

    esp_err_t nvs_error = nvs_entry_next(&iterator);
    if (nvs_error == ESP_OK)
    {
        nvs_entry_info_t entry_info;
        nvs_entry_info(iterator, &entry_info);

        uint16_t param_value;
        nvs_get_u16(handle, entry_info.key, &param_value);

        nvs_data.key = entry_info.key;
        nvs_data.value = param_value;
    }
    else
        nvs_release_iterator(iterator);

    return nvs_data;
}
