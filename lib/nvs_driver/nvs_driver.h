#ifndef NVS_DRIVER_H
#define NVS_DRIVER_H

#include <stdint.h>

typedef uint8_t nvs_err_t;

#define NVS_OK 0
#define NVS_INIT_ERROR 1

typedef struct
{
    char* key;
    uint16_t value;
} nvs_data_t;

const nvs_data_t get_first_nvs_data(void);
const nvs_data_t get_next_nvs_data(void);
nvs_err_t nvs_write_u16(const char* key, uint16_t value);

nvs_err_t nvs_init(void);
//nvs_err_t nvs_open_file(const char* namespace, nvs_handle_t* handle);
//void nvs_close_file(nvs_handle_t handle);

#endif // NVS_DRIVER_H
