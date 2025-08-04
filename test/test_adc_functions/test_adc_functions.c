#include <unity.h>

#include "adc_functions.h"
#include "adc_registers.h"
#include "spi_functions.h"
#include "calibration_params.c"

#include "mock_adc_rw.h"

void setUp(void)
{
// set stuff up here
}

void tearDown(void)
{
// clean stuff up here
}

const uint16_t data_array[] = {0xFFC4, 0xFFCA, 0xFFCA, 0xFFDC, 0xFFC5};

int callback(struct adc_register* reg, int numcalls)
{
    (*reg).data = data_array[numcalls];
    return 0;
}

void test_average_data_fm_register()
{
    struct adc_register U_RMS =
    {
        .address = 0x47,
        .data = 0
    };
    read_adc_register_Stub(callback);

    adc_data average = get_average_data_fm_register(U_RMS, 5);
    TEST_ASSERT(average == 0xFFCB);
}