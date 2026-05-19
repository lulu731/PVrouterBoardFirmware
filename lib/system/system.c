#include "system.h"

#include "adc_rw.h"

#include <stdint.h>

extern struct adc_register P_MEAN;

/**
 * @brief Read the main real power in mW from the adc register P_MEAN
 * @return The main real power in mW. If the sign bit is set, the value is negative.
 */
int16_t get_main_real_power()
{
    read_adc_register(&P_MEAN);
    if (P_MEAN.data >> 15 == 1)
        return -(~P_MEAN.data);
    return P_MEAN.data;
}