#include "spi_master.h"
#include "spi_config.h"
#include "spi_functions.h"
#include "adc_adress.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

esp_err_t read_data(const adc_address adress, uint16_t* data)
{
    spi_transaction_t trans = {
        .addr = adress | MSB_READ_ADDR_MASK,
        .length = 24,
        .rxlength = 16,
        .flags = SPI_TRANS_USE_RXDATA,
    };

    esp_err_t ret = spi_device_polling_transmit( meter_handle, &trans);
    if (ret ==ESP_OK)
        *data = (uint16_t)trans.rx_data[0] << 8 | (uint16_t)trans.rx_data[1];

    return ret;
}


esp_err_t write_data(const adc_address adress, const uint16_t data)
{
    spi_transaction_t trans = {
        .addr = adress,
        .length = 16,
        .flags = SPI_TRANS_USE_TXDATA,
    };
    set_txdata_with(trans.tx_data, data);

    return spi_device_polling_transmit( meter_handle, &trans);
}


void write_PL_constant()
{}


void write_MMODE()
{}


void exec_calibrationL(){}
void exec_gain_calibrationL(){}
void exec_angle_calibrationL(){}
void write_PStartTh(){}
void write_PNolTh(){}
void write_QStartTh(){}
void write_QNolTh(){}
void write_Ugain(){}
void write_IgainL(){}
void write_IoffsetL(){}
void write_Uoffset(){}

//N line
void exec_calibrationN(){}
void exec_gain_calibrationN(){}
void exec_angle_calibrationN(){}
void write_IgainN(){}
void write_IoffsetN(){}

void exec_calibration(void)
{
    write_data(CAL_START, CAL_NEEDED); //start calibration

    ///update CS1 register
    adc_param cs_data;
    read_data(CS1, &cs_data);
    write_data(CS1, cs_data);

    write_PL_constant();
    write_MMODE();

    exec_calibrationL();
    exec_gain_calibrationL();
    exec_angle_calibrationL();
    write_PStartTh();
    write_PNolTh();
    write_QStartTh();
    write_QNolTh();
    write_Ugain();
    write_IgainL();
    write_IoffsetL();
    write_Uoffset();

    //N line
    exec_calibrationN();
    exec_gain_calibrationN();
    exec_angle_calibrationN();
    write_IgainN();
    write_IoffsetN();

    write_data(CAL_START, CAL_END); //end calibration
}

esp_err_t test_spi()
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing bus SPI%d...", EMETER_HOST + 1);
    //Initialize the SPI bus
    ret = spi_bus_initialize(EMETER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);


    ESP_LOGI(TAG, "Initializing meter...");
    ret = spi_bus_add_device( EMETER_HOST, &devcfg, &meter_handle);
    ESP_ERROR_CHECK(ret);


    exec_calibration();

    return ESP_FAIL;//todo: to be corrected
}