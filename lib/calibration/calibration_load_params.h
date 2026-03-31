#ifndef CALIBRATION_LOAD_PARAMS_H
#define CALIBRATION_LOAD_PARAMS_H

#ifdef TEST
    #include <stdint.h>
    #include <stdbool.h>
    /**
    * @brief Load a parameter from to_param_array with the value of the given key.
    *
    * @param key The key to search for in the non-volatile storage.
    * @param key_array An array of key strings available for this parameter. The
    *                  length of this array must be the same as to_param_array.
    * @param value The value to store in the param.
    * @param to_param_array An array of pointers to uint16_t params. If the key is found,
    *                       the value will be stored in the corresponding element of this array.
    * @param array_keys_count The length of the key_array or to_param_array.
    * @return true if the key was found and loaded, false otherwise.
    */
    bool load_param(const char*    key,   const char* key_array[],
                    const uint16_t value, uint16_t*   to_param_array[],
                    const int      array_keys_count);
#endif

int load_calibration_params(void);

#endif // CALIBRATION_LOAD_PARAMS_H
