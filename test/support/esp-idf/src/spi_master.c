#include <driver/spi_master.h>

#include <esp_err.h>

int write_adc_caller;
int write_adc_register_call_count = 0;
int nbr_access_to_adc = 0;

esp_err_t spi_device_polling_transmit(spi_device_handle_t handle, spi_transaction_t *trans_desc)
{
    if (write_adc_caller)
        write_adc_register_call_count++;
    nbr_access_to_adc++;
    if (nbr_access_to_adc > 7)
    {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t calibrate_adc(void)
{
    return ESP_OK;
}
