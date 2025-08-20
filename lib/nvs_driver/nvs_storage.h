#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H

#include "nvs_driver.h"

#include "esp_err.h"
#include "nvs.h"

#include <stdint.h>

#define NVS_STORAGE_OK    0
#define NVS_STORAGE_ERROR 1

void nvs_storage_create(const char* namespace);
nvs_err_t nvs_storage_open();
void nvs_storage_close();

#endif // NVS_STORAGE_H
