#include "adc_rw.h"

#include "adc_registers.h"
#include "spi_functions.h"

#include "esp_err.h"
#include "driver/spi_master.h"

#define MSB_READ_ADDR_MASK 0x80 //adress should start with 1 for read phase

extern spi_device_handle_t meter_handle;

/***********************/
/* READ and WRITE data */
/***********************/

int read_adc_register(struct adc_register* reg)
{
    spi_transaction_t trans = {
        .addr = (*reg).address | MSB_READ_ADDR_MASK,
        .length = 24,
        .rxlength = 16,
        .flags = SPI_TRANS_USE_RXDATA,
    };

    esp_err_t ret = spi_device_polling_transmit( meter_handle, &trans);
    if (ret ==ESP_OK)
        (*reg).data = (adc_data)trans.rx_data[0] << 8 | (adc_data)trans.rx_data[1];

    return ret;
}

int write_adc_register(const struct adc_register reg)
{
    spi_transaction_t trans = {
        .addr = reg.address,
        .length = 16,
        .flags = SPI_TRANS_USE_TXDATA,
    };
    set_txdata_with(trans.tx_data, reg.data);

    return spi_device_polling_transmit( meter_handle, &trans);
}

//todo: manage errors in writing
/***********************/
/*        END          */
/* READ and WRITE data */
/***********************/
