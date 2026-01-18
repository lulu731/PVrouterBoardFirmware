#include "calibration_load_params.h"

#include "nvs_driver.h"

#include "calibration_params.h"

#include <stddef.h>
#include <string.h>

/**
 * @brief Loads a parameter from non-volatile storage to the given array.
 *
 * @param key The key to search for in the non-volatile storage.
 * @param key_array An array of key strings available for this parameter. The length of this array must be the same as to_param_array.
 * @param value The value to store in the param.
 * @param to_param_array An array of pointers to uint16_t params. If the key is found, the value will be stored in the corresponding element of this array.
 * @param array_keys_count The length of the key_array or to_param_array.
 */
void load_param(const char*    key,   const char* key_array[],
                const uint16_t value, uint16_t*   to_param_array[],
                const int      array_keys_count)
{
    for (int i = 0; i < array_keys_count; i++)
        if (strcmp(key, key_array[i]) == 0)
        {
            *to_param_array[i] = value;
            return;
        }
}

/**
 * @brief Loads all calibration parameters from non-volatile storage into the given array.
 *
 * @return The number of parameters loaded.
 */
int load_calibration_params(void)
{
    int data_count = 0;

    nvs_data_t nvs_data = get_first_nvs_data();

    while (nvs_data.key != NULL)
    {
        load_param(nvs_data.key, string_keys_array, nvs_data.value, keys_array, keys_count);
        data_count++;

        nvs_data = get_next_nvs_data();
    }

    return data_count;
}