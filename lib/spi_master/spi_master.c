#include "spi_master.h"
#include "spi_config.h"

#include "calibration.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

esp_err_t calibrate_adc()
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

    return ret;//todo: return error?
}