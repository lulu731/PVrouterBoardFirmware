#include "spi_functions.h"

#include <stdint.h>
#include <math.h>

void set_txdata_with(uint8_t tx_data[4], const adc_data data)
{
    tx_data[1] = (uint8_t)data;
    tx_data[0] = (uint8_t)(data>>8);
}


void split_uint32_to_uint8_array(const uint32_t data, uint16_t receiver[2])
{
    receiver[1] = (uint16_t)data;
    receiver[0] = (uint16_t)(data>>16);
}


uint16_t get_mmode_value(uint16_t Lgain, uint16_t Ngain, uint16_t LNsel,
                         uint16_t DisHPF, uint16_t Amod, uint16_t Rmod,
                         uint16_t Zxcon, uint16_t Pthresh)
{
    return (uint16_t)(Lgain<<13 | Ngain<<11 | LNsel<<10
                      | DisHPF<<8 | Amod<<7 | Rmod<<6
                      | Zxcon<<4 | Pthresh);
}

uint16_t get_average_data_in_array( const uint16_t data_array[], const uint8_t array_size)
{
    uint32_t data = 0;

    for (uint8_t i = 0; i < array_size; i++)
    {
        data += data_array[i];
    }
    return data / array_size;
}

extern uint16_t Gl, Vl, Vu;

uint16_t get_threshold(const uint8_t K)
{
    return roundf(93.2067556 * (float)(Gl * Vl *Vu * K/1000));
}

uint16_t get_offset_from_measured(const uint16_t measured_value, const uint16_t gain)
{
    float value = measured_value / 1000;
    return value * ((float)gain / (1 << 16) * (1<<8));
}