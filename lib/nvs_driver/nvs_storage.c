#include "nvs_storage.h"

#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "nvs_file";
static const char* nvs_namespace;
static nvs_handle_t handle;

/**
 * @brief Create a new NVS storage with the given namespace.
 *
 * This function initializes the NVS storage with the given namespace.
 *
 * @param namespace The namespace to use for the NVS storage.
 */
void nvs_storage_create(const char* namespace)
{
    nvs_namespace = namespace;
    handle = 0;
}

/**
 * @brief Open the NVS storage with the given namespace.
 *
 * This function opens the NVS storage with the given namespace.
 * If the opening fails, it returns @ref NVS_STORAGE_ERROR. Otherwise, it returns @ref NVS_STORAGE_OK.
 *
 * @return NVS_STORAGE_OK if the opening succeeds, NVS_STORAGE_ERROR if it fails.
 */
nvs_err_t nvs_storage_open()
{
    ESP_LOGI(TAG, "nvs_namespace in nvs_storage_open = %s\n", nvs_namespace);
    esp_err_t nvs_error = nvs_flash_init_partition("config");
    if (nvs_error != ESP_OK)
    {
        ESP_LOGE(TAG, "nvs_error flash_init = %s", esp_err_to_name(nvs_error));
        return NVS_STORAGE_ERROR;
    }

    nvs_error = nvs_open(nvs_namespace, NVS_READWRITE, &handle);
    if (nvs_error != ESP_OK)
    {
        ESP_LOGE(TAG, "nvs_error open = %s", esp_err_to_name(nvs_error));
        return NVS_STORAGE_ERROR;
    }
    return NVS_STORAGE_OK;
}

/**
 * @brief Close the NVS storage.
 *
 * This function closes the NVS storage.
 *
 */
void nvs_storage_close()
{
    nvs_close(handle);
}

/**
 * @brief Save data to NVS storage.
 *
 * This function saves the given data to the NVS storage.
 *
 * @param data The nvs_data_t structure containing the key and value to save.
 * @return NVS_STORAGE_OK if the save succeeds, NVS_STORAGE_ERROR if it fails.
 */
nvs_err_t nvs_storage_save(nvs_data_t data)
{
    esp_err_t nvs_error = nvs_set_u16(handle, data.key, data.value);
    if (nvs_error != ESP_OK)
    {
        return NVS_STORAGE_ERROR;
    }
    nvs_error = nvs_commit(handle);
    if (nvs_error != ESP_OK)
    {
        return NVS_STORAGE_ERROR;
    }
    return NVS_STORAGE_OK;
}
