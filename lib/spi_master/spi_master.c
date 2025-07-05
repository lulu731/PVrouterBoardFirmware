#include "spi_master.h"
#include "spi_config.h"
#include "spi_functions.h"
#include "spi_adress.h"

#include "esp_log.h"

static const char TAG[] = "spi_master.c";

spi_device_handle_t meter_handle;

esp_err_t read_data(const addr adress, uint16_t* data)
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


esp_err_t write_data(const addr adress, const uint16_t data)
{
    spi_transaction_t trans = {
        .addr = adress,
        .length = 16,
        .flags = SPI_TRANS_USE_TXDATA,
    };
    set_txdata_with(trans.tx_data, data);

    return spi_device_polling_transmit( meter_handle, &trans);
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

    uint16_t data;
    esp_err_t err = read_data(0x02, &data);

    if (err == ESP_OK)
    {
        if (data == 0x000C)
            ESP_LOGI(TAG, "Read data is OK (=12): %d", data);
        else
        {
            ESP_LOGI(TAG, "Status read data (should be 12): %d", data);
        }
    }
    else
    {
        ESP_LOGI(TAG, "error in read polling_transmit: %s", esp_err_to_name(err));
        return err;
    }


    //write value
    err = write_data(0x02, 0x0020);

    if (err == ESP_OK)
        ESP_LOGI(TAG, "Write is OK (=32)");
    else
    {
        ESP_LOGI(TAG, "error in write polling_transmit: %s", esp_err_to_name(err));
        return err;
    }

    //Read value 32
    uint16_t new_data;
    err = read_data(0x02, &new_data);

    if (err == ESP_OK)
    {
        if (new_data == 0x20)
            ESP_LOGI(TAG, "Read data is OK (=32): %d", new_data);
        else
        {
            ESP_LOGI(TAG, "Status new_data should be 32: %d", new_data);
        }
    }
    else
    {
        ESP_LOGI(TAG, "error in read polling_transmit: %s", esp_err_to_name(err));
        return err;
    }

    return err;
}