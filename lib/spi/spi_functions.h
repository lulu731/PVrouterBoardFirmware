#ifndef SPI_FUNCTIONS_H
#define SPI_FUNCTIONS_H

#include "adc_param.h"

#include <stdint.h>

void     set_txdata_with(uint8_t tx_data[4], const adc_param data);
void     split_uint32_to_uint8_array(const uint32_t data, uint16_t receiver[2]);
uint16_t get_mmode_value(uint8_t Lgain, uint8_t Ngain, uint8_t LNsel,
                         uint8_t DisHPF, uint8_t Amod, uint8_t Rmod,
                         uint8_t Zxcon, uint8_t Pthresh);

#endif // SPI_FUNCTIONS_H
