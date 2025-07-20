#ifndef NVS_FUNCTIONS_H
#define NVS_FUNCTIONS_H

#include <stdint.h>

/**
 * @brief Load a parameter from to_param_array with the value of the given key.
 *
 * @param key The key to search for in the non-volatile storage.
 * @param key_array An array of key strings available or this parameter. The
 *                  length of this array must be the same as to_param_array.
 * @param value The value to store in the param.
 * @param to_param_array An array of pointers to uint16_t params. If the key is found,
 *                       the value will be stored in the corresponding element of this array.
 */
void load_param(const char*    key,   const char* key_array[],
                const uint16_t value, uint16_t*   to_param_array[]);


#endif // NVS_FUNCTIONS_H
