#ifndef ADC_FUNCTIONS_H
#define ADC_FUNCTIONS_H

#include "adc_registers.h"

adc_data get_average_data_fm_register(struct adc_register* reg, const int nbr_datas);

adc_data get_complement_average_fm_register(struct adc_register* source_reg, const int nbr_datas);

void write_PL_constant(); //21 - 22H

/// set calibration gains and angles to 0
void write_gain_register(const struct adc_register gain_register); // 23H or 25H
void write_phi_register(const struct adc_register gain_register); // 24H or 26H

void write_threshold_register(struct adc_register threshold_register);

void write_MMODE();

/*---------------gain------------------*/

/**
 * @brief get calculated gain based on measured and expected values to gain address
 *
 * @param expected_value expected value of measured value
 * @param measured_value_register register of measured value
 * @param gain_register register of gain value
 */
 adc_data get_line_gain(const float expected_value, struct adc_register* measured_value_register,
                        struct adc_register* gain_register);

/*---------------ofsset------------------*/

adc_data get_offset(struct adc_register reg, const adc_data gain);
adc_data get_power_offset(struct adc_register* power_register);

#endif // ADC_FUNCTIONS_H
