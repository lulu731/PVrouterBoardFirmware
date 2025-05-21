#include "spi_master.h"
#include "spi_config.h"

#include "esp_log.h"

static const char TAG[] = "main";

esp_err_t test_spi()
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing bus SPI%d...", EMETER_HOST + 1);
    //Initialize the SPI bus
    ret = spi_bus_initialize(EMETER_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    spi_device_handle_t meter_handle;

    ESP_LOGI(TAG, "Initializing meter...");
    ret = spi_bus_add_device( EMETER_HOST, &devcfg, &meter_handle);
    ESP_ERROR_CHECK(ret);

    spi_transaction_t trans = {
        .addr = 0x02 | MSB_ADDR_MASK,
        .rxlength = 16,
        .flags = SPI_TRANS_USE_RXDATA,
    };

    esp_err_t err = spi_device_polling_transmit( meter_handle, &trans);

    if (err == ESP_OK)
    {
        if (trans.rx_data[0] == 0x0C)
            ESP_LOGI(TAG, "Read data is OK (=12):%d", trans.rx_data[0]);
        else
        {
            ESP_LOGI(TAG, "Status read 0: %d", trans.rx_data[0]);
            ESP_LOGI(TAG, "Status read 1: %d", trans.rx_data[1]);
        }
    }
    else
    {
        ESP_LOGI(TAG, "error: %s", esp_err_to_name(err));
        return err;
    }

    //write value
    trans.rxlength = 0;
    trans.length = 16;
    trans.tx_data[0] = 0x20;
    trans.flags = SPI_TRANS_USE_TXDATA;

    err = spi_device_polling_transmit( meter_handle, &trans);

    if (err == ESP_OK)
        ESP_LOGI(TAG, "Write is OK (=32): %d", trans.rx_data[0]);
    else
    {
        ESP_LOGI(TAG, "error: %s", esp_err_to_name(err));
        return err;
    }

    //Read value 32
    trans.rxlength = 16;
    trans.flags = SPI_TRANS_USE_RXDATA;

    err = spi_device_polling_transmit( meter_handle, &trans);

    if (err == ESP_OK)
    {
        if (trans.rx_data[0] == 0x20)
            ESP_LOGI(TAG, "Read data is OK (=32):%d", trans.rx_data[0]);
        else
        {
            ESP_LOGI(TAG, "Status read 0: %d", trans.rx_data[0]);
            ESP_LOGI(TAG, "Status read 1: %d", trans.rx_data[1]);
        }
    }
    else
    {
        ESP_LOGI(TAG, "error: %s", esp_err_to_name(err));
        return err;
    }

    return err;
}
