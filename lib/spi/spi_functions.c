#include "spi_functions.h"

#include <stdint.h>

void set_txdata_with(uint8_t tx_data[4], const uint16_t data)
{
    uint8_t* pdata = (uint8_t*)(&data);

    tx_data[1] = *pdata;
    pdata++;
    tx_data[0] = *pdata;
}
