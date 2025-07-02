#include "spi_functions.h"
#include "spi_param.h"

#include <stdint.h>

void set_txdata_with(uint8_t tx_data[4], spi_param data)
{
    uint8_t* pdata = (uint8_t*)(&data);

    tx_data[1] = *pdata;
    pdata++;
    tx_data[0] = *pdata;
}
