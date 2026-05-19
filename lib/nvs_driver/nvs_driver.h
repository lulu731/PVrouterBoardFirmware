#ifndef NVS_DRIVER_H
#define NVS_DRIVER_H

#include <stdint.h>

typedef uint8_t nvs_err_t;

#define NVS_OK         0
#define NVS_INIT_ERROR 1

typedef struct
{
    char* key;
    uint16_t value;
} nvs_data_t;

nvs_data_t get_first_nvs_data(void);
nvs_data_t get_next_nvs_data(void);

//nvs_err_t  nvs_init(void);

#endif // NVS_DRIVER_H
