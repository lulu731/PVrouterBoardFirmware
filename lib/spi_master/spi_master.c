#include "spi_master.h"
#include "spi_config.h"

#include "calibration.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

/**
 * @brief Initialize SPI bus and meter, and execute meter calibration
 *
 * This function initializes the SPI bus and adds the meter device to the bus.
 * It then calls the exec_calibration() function to execute the meter calibration.
 *
 * @return ESP_OK if the calibration was successful, ESP_ERR_* otherwise
 */
esp_err_t calibrate_adc()
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing bus SPI%d...", EMETER_HOST + 1);
    //Initialize the SPI bus
    ret = spi_bus_initialize(EMETER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_ERROR_CHECK(ret);
    }
    else ESP_LOGI(TAG, "SPI bus initialied");

    ESP_LOGI(TAG, "Initializing meter...");
    ret = spi_bus_add_device(EMETER_HOST, &devcfg, &meter_handle);
        if (ret != ESP_OK)
    {
        ESP_ERROR_CHECK(ret);
    }
    else ESP_LOGI(TAG, "SPI meter initialied meter_handle = %d", meter_handle);

    exec_calibration();

    return ret;//todo: return error?
}