#ifndef NVS_FUNCTIONS_H
#define NVS_FUNCTIONS_H

#include "esp_err.h"
#include "nvs.h"

#include <stdint.h>

esp_err_t nvs_init(void);
esp_err_t nvs_open_file(const char* namespace, nvs_handle_t* handle);
void nvs_close_file(nvs_handle_t handle);

#endif // NVS_FUNCTIONS_H
