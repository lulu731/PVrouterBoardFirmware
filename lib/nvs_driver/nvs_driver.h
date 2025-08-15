#ifndef NVS_DRIVER_H
#define NVS_DRIVER_H

#include <stdint.h>

typedef struct
{
    char* key;
    uint16_t value;
} nvs_data_t;

const nvs_data_t get_first_nvs_data(void);
const nvs_data_t get_next_nvs_data(void);

#endif // NVS_DRIVER_H
