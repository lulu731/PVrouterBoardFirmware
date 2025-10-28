#include <driver/spi_master.h>

int nbr_access_to_adc = 0;

esp_err_t spi_device_polling_transmit(spi_device_handle_t handle, spi_transaction_t *trans_desc)
{
    nbr_access_to_adc++;
    if (nbr_access_to_adc > 7)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}
