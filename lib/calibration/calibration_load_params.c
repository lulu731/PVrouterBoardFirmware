#include "calibration_load_params.h"

#include "nvs_driver.h"

#include "calibration_params.h"

#include "esp_log.h"

#include <stddef.h>
#include <string.h>

#define TAG "calibration_load_params"

/**
 * @brief Loads a parameter from non-volatile storage to the given array.
 *
 * @param key The key to search for in the non-volatile storage.
 * @param key_array An array of key strings available for this parameter. The length of this array must be the same as to_param_array.
 * @param value The value to store in the param.
 * @param to_param_array An array of pointers to uint16_t params. If the key is found, the value will be stored in the corresponding element of this array.
 * @param array_keys_count The length of the key_array or to_param_array.
 * @return true if the key was found and loaded, false otherwise.
 */
bool load_param(const char*    key,   const char* key_array[],
                const uint16_t value, uint16_t*   to_param_array[],
                const int      array_keys_count)
{
    for (int i = 0; i < array_keys_count; i++)
        if (strcmp(key, key_array[i]) == 0)
        {
            *to_param_array[i] = value;
            return true;
        }
    return false;
}

/**
 * @brief Loads all calibration parameters from non-volatile storage into the given array.
 *
 * @return The number of parameters loaded.
 */
int load_calibration_params(void)
{
    int data_count = 0;
    int unmatched_count = 0;

    nvs_data_t nvs_data = get_first_nvs_data();

    while (nvs_data.key != NULL)
    {
        bool found = load_param(nvs_data.key, string_keys_array, nvs_data.value, keys_array, keys_count);
        if (!found)
        {
            ESP_LOGW(TAG, "Unknown calibration key in NVS: '%s' (value: %u)", nvs_data.key, nvs_data.value);
            unmatched_count++;
        }
        data_count++;

        nvs_data = get_next_nvs_data();
    }

    if (unmatched_count > 0)
    {
        ESP_LOGW(TAG, "Found %d unknown key(s) in NVS that were not loaded", unmatched_count);
    }

    return data_count;
}
