#include "spi_functions.h"
#include "adc_param.h"

#include <stdint.h>

void set_txdata_with(uint8_t tx_data[4], const adc_param data)
{
    tx_data[1] = (uint8_t)data;
    tx_data[0] = (uint8_t)(data>>8);
}


void split_uint32_to_uint8_array(const uint32_t data, uint16_t receiver[2])
{
    receiver[1] = (uint16_t)data;
    receiver[0] = (uint16_t)(data>>16);
}


uint16_t get_mmode_value(uint8_t Lgain, uint8_t Ngain, uint8_t LNsel,
                         uint8_t DisHPF, uint8_t Amod, uint8_t Rmod,
                         uint8_t Zxcon, uint8_t Pthresh)
{
    return (uint16_t)(Lgain<<13 | Ngain<<11 | LNsel<<10
                      | DisHPF<<8 | Amod<<7 | Rmod<<6
                      | Zxcon<<4 | Pthresh);
}