#include "nvs_functions.h"

#include <stdint.h>
#include <string.h>

void load_param(const char *key, const char *key_array[],
                const uint16_t value, uint16_t *to_param_array[])
{
    for (int i = 0; i < 3; i++)
        if (strcmp(key, key_array[i]) == 0)
        {
            *(to_param_array[i]) = value;
            return;
        }
}
