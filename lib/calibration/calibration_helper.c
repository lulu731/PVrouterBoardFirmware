#include "calibration_helper.h"

#include "nvs_driver.h"

#include "calibration_params.h"

#include <stddef.h>
#include <string.h>

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