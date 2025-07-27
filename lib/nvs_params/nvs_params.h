#ifndef NVS_PARAMS_H
#define NVS_PARAMS_H

#include <stdint.h>

void load_nvs_params(void);
void save_nvs_param(const char* key, const uint16_t value);

#endif // NVS_PARAMS_H
