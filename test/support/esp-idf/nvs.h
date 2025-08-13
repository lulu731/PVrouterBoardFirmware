#ifndef NVS_H
#define NVS_H

#include "esp_err.h"

#include <stdint.h>

#define NVS_KEY_NAME_MAX_SIZE 16
#define NVS_NS_NAME_MAX_SIZE  NVS_KEY_NAME_MAX_SIZE

typedef uint32_t nvs_handle_t;

typedef uint32_t nvs_iterator_t;

typedef enum {
    NVS_TYPE_U16   = 0x02,  /*!< Type uint16_t */
} nvs_type_t;

typedef struct {
    char namespace_name[NVS_NS_NAME_MAX_SIZE];  /*!< Namespace to which key-value belong */
    char key[NVS_KEY_NAME_MAX_SIZE];            /*!< Key of stored key-value pair */
    nvs_type_t type;                            /*!< Type of stored key-value pair */
} nvs_entry_info_t;

esp_err_t nvs_entry_find(const char *part_name,
        const char *namespace_name,
        nvs_type_t type,
        nvs_iterator_t *output_iterator);

esp_err_t nvs_entry_info(const nvs_iterator_t iterator, nvs_entry_info_t *out_info);

esp_err_t nvs_get_u16 (nvs_handle_t handle, const char* key, uint16_t* out_value);

#endif // NVS_H
