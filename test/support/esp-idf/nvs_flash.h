#ifndef NVS_FLASH_H
#define NVS_FLASH_H

#include "esp_err.h"

esp_err_t nvs_flash_init(void);
esp_err_t nvs_flash_init_partition(const char *partition_label);

#endif // NVS_FLASH_H
