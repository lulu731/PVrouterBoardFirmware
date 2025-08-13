#include "nvs_driver.h"

#include "nvs.h"

#include <stddef.h>

static const char* namespace = "meter_config";

nvs_handle_t handle = 0;
static nvs_iterator_t iterator = 0; // TODO: should be released nvs_release_iterator(iterator);

nvs_data_t get_first_nvs_data(void)
{
    nvs_data_t nvs_data =
    {
        .key = NULL,
        .value = 0
    };

    esp_err_t nvs_error = nvs_entry_find("nvs", namespace, NVS_TYPE_U16, &iterator);
    if (nvs_error == ESP_OK)
    {
        nvs_entry_info_t entry_info;
        nvs_entry_info(iterator, &entry_info);

        uint16_t param_value;
        nvs_get_u16(handle, entry_info.key, &param_value);

        nvs_data.key = entry_info.key;
        nvs_data.value = param_value;
    }

    return nvs_data;
}
