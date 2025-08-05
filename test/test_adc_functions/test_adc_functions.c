#include <unity.h>

#include "adc_functions.h"
#include "adc_registers.h"
#include "spi_functions.h"
#include "calibration_params.c"
#include "adc_registers.c"

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
    read_adc_register_Stub(callback);

    adc_data average = get_average_data_fm_register(U_RMS, 5);
    TEST_ASSERT(average == 0xFFCB);
}

extern uint16_t Mc, Un, Ib, Gl, Vl, Vu;

void test_write_PL_constant()
{
    Mc = 1000;
    Un = 230;
    Ib = 10;
    Gl = 1;
    Vl = 167;
    Vu = 529;

    PL_CONST_H.data = 0x01eb;
    write_adc_register_ExpectAndReturn(PL_CONST_H, 0);
    PL_CONST_L.data = 0xa5e4;
    write_adc_register_ExpectAndReturn(PL_CONST_L, 0);

    write_PL_constant();
}

extern struct adc_register L_GAIN, N_GAIN, L_PHI, N_PHI;
void test_write_null_gains()
{
    L_GAIN.data = 0;
    N_GAIN.data = 0;

    write_adc_register_ExpectAndReturn(L_GAIN, 0);
    write_adc_register_ExpectAndReturn(N_GAIN, 0);

    write_gain_register(L_GAIN);
    write_gain_register(N_GAIN);
}