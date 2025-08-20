#include "nvs_storage.h"

static const char *TAG = "nvs_file";
static const char* nvs_namespace;
static nvs_handle_t handle;

void nvs_storage_create(const char* namespace)
{
    nvs_namespace = namespace;
    handle = 0;
}

nvs_err_t nvs_storage_open()
{
    esp_err_t nvs_error = nvs_open(nvs_namespace, NVS_READWRITE, &handle);
    if (nvs_error != ESP_OK)
    {
        return NVS_STORAGE_ERROR;
    }
    return NVS_STORAGE_OK;
}

void nvs_storage_close()
{
    nvs_close(handle);
}
