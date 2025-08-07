#ifndef ADC_FUNCTIONS_H
#define ADC_FUNCTIONS_H

#include "adc_registers.h"

adc_data get_average_data_fm_register(struct adc_register reg, const int nbr_datas);

adc_data get_complement_average_fm_register(struct adc_register source_reg, const int nbr_datas);

void write_PL_constant(); //21 - 22H

/// set calibration gains and angles to 0
void write_gain_register(const struct adc_register gain_register); // 23H or 25H
void write_phi_register(const struct adc_register gain_register); // 24H or 26H

void write_threshold_register(struct adc_register threshold_register);

/**
 * @brief write calculated gain based on measured and expected values to gain address
 *
 * @param expected_value expected value of measured value
 * @param measured_value_address address of measured value
 * @param gain_address address of gain value
 */
 adc_data get_line_gain(const float expected_value, struct adc_register measured_value_register,
                        struct adc_register gain_register);

/*---------------gain------------------*/

void get_Ugain();

void write_Ugain(); // 31H

void get_IgainL();

void write_IgainL(); // 32H

void get_IgainN();

void write_IgainN(); // 33H

/*---------------ofsset------------------*/

uint16_t get_offset(struct adc_register reg, const uint16_t gain);

void get_Uoffset();

void write_Uoffset(); // 34H

void get_IoffsetL();

void write_IoffsetL(); // 35H

void get_IoffsetN();

void write_IoffsetN(); // 36H

void get_PQoffsetL(); // 37H 38H

void write_PQoffsetL(); // 37H 38H

void get_PQoffsetN();

void write_PQoffsetN(); // 37H 38H

#endif // ADC_FUNCTIONS_H
