#ifndef SPI_FUNCTIONS_H
#define SPI_FUNCTIONS_H

#include "adc_param.h"
#include "adc_adress.h"

#include <stdint.h>

extern uint16_t Gl;
extern uint16_t Vl;
extern uint16_t Vu;

void     set_txdata_with(uint8_t tx_data[4], const adc_param data);
void     split_uint32_to_uint8_array(const uint32_t data, uint16_t receiver[2]);
uint16_t get_mmode_value(uint16_t Lgain, uint16_t Ngain, uint16_t LNsel,
                         uint16_t DisHPF, uint16_t Amod, uint16_t Rmod,
                         uint16_t Zxcon, uint16_t Pthresh);
uint16_t get_average_value(const uint16_t data_array[], const uint8_t array_size);

uint16_t get_threshold(const uint8_t K); //K in per mille

uint16_t get_offset_from_measured(const uint16_t measured_value, const uint16_t gain);

#endif // SPI_FUNCTIONS_H
