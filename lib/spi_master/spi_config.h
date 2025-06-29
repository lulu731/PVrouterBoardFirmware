#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "driver/spi_master.h"

#define EMETER_HOST       SPI2_HOST
#define PIN_NUM_MISO      13
#define PIN_NUM_MOSI      11
#define PIN_NUM_CLK       12
#define PIN_NUM_CS        10

#define MSB_READ_ADDR_MASK 0x80 //adress should start with 1 for read phase

spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 24,
        .flags = 0,
    };

    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 8,
        .dummy_bits = 0,
        .mode = 3,                              //SPI mode 3
        .spics_io_num = PIN_NUM_CS,             //CS pin
        .queue_size = 1,                        //number transactions to be queued at a time
        .clock_speed_hz = 160 * 1000
    };

#endif // SPI_CONFIG_H
