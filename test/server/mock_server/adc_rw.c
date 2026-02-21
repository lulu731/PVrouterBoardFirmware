#include "adc_rw.h"

#include "esp_err.h"

#define MSB_READ_ADDR_MASK 0x80 //adress should start with 1 for read phase

/***********************/
/* READ and WRITE data */
/***********************/

int read_adc_register(struct adc_register* reg)
{
    return ESP_OK;
}

int write_adc_register_call_count = 0;
int write_adc_register(const struct adc_register reg)
{
    return write_adc_register_call_count++;
}

//todo: manage errors in writing
/***********************/
/*        END          */
/* READ and WRITE data */
/***********************/
