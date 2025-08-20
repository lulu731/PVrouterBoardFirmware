#include <unity.h>

#include "adc_functions.h"
#include "adc_registers.h"
#include "calibration_helpers.h"
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

    adc_data average = get_average_data_fm_register(&U_RMS, 5);
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

extern struct adc_register U_RMS, U_GAIN;

void test_get_line_gain_for_Un(void)
{
    float Un = 220.024;

    U_RMS.data = 0x6019;
    struct adc_register measured_value_register = U_RMS;
    read_adc_register_ExpectAndReturn(&measured_value_register, 0);

    U_GAIN.data = 26400;
    struct adc_register gain_register = U_GAIN;
    read_adc_register_ExpectAndReturn(&gain_register, 0);

    adc_data expected_gain = 0x5C3B;

    adc_data actual_gain = get_line_gain(Un, &measured_value_register, &gain_register);

    TEST_ASSERT_EQUAL_UINT16(expected_gain, actual_gain);
}

void test_get_line_gain_for_Ib(void)
{
    float Ib = 5.008;

    I_RMS.data = 0x1A58;
    struct adc_register measured_value_register = I_RMS;
    read_adc_register_ExpectAndReturn(&measured_value_register, 0);

    I_GAIN_L.data = 31251;
    struct adc_register gain_register = I_GAIN_L;
    read_adc_register_ExpectAndReturn(&gain_register, 0);

    adc_data expected_gain = 0x5AA6;

    adc_data actual_gain = get_line_gain(Ib, &measured_value_register, &gain_register);

    TEST_ASSERT_EQUAL_UINT16(expected_gain, actual_gain);
}
