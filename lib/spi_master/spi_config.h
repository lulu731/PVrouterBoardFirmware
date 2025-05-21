#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "driver/spi_master.h"

#define EMETER_HOST       SPI2_HOST
#define PIN_NUM_MISO      13
#define PIN_NUM_MOSI      12
#define PIN_NUM_CLK       11
#define PIN_NUM_CS        10

#define MSB_ADDR_MASK 0x80 //adress should start with 1

spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
//        .max_transfer_sz =
        .flags = 0,
    };

    spi_device_interface_config_t devcfg = {
        .address_bits = 8,
        .mode = 2,                              //SPI mode 2
        .spics_io_num = PIN_NUM_CS,             //CS pin
        .queue_size = 1                        //number transactions to be queued at a time
    };

#endif // SPI_CONFIG_H
