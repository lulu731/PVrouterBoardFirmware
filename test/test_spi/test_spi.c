#include <unity.h>

#include "spi_functions.h"
#include "adc_registers.h"

void setUp(void)
{
// set stuff up here
}

void tearDown(void)
{
// clean stuff up here
}

void test_tx_data()
{
    extern adc_data CAL_NEEDED;
    const adc_data data = CAL_NEEDED;
    uint8_t tx_data[4];
    uint8_t expected_data[2] = {0x56, 0x78};

    set_txdata_with(tx_data, data);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data, tx_data, 2);
}

void test_conv_uint32_to_uint8_array()
{
    const uint32_t data = 0xABCDEF01;
    uint16_t split_array[2];
    split_uint32_to_uint8_array(data, split_array);

    uint16_t expected_array[2] = {0xABCD, 0xEF01};

    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected_array, split_array, 2);
}

void test_get_mmode_value()
{
    uint16_t Lgain = 0b100;
    uint16_t Ngain = 0;
    uint16_t LNsel = 0;
    uint16_t DisHPF = 0;
    uint16_t Amod = 0;
    uint16_t Rmod = 0;
    uint16_t Zxcon = 0b10;
    uint16_t Pthresh = 0b10;
    uint16_t result = get_mmode_value(Lgain, Ngain, LNsel, DisHPF, Amod, Rmod, Zxcon, Pthresh);

    TEST_ASSERT(result == 0b1000000000100010);
}

void test_average_data()
{
    const uint8_t count = 5;
    const uint16_t data_array[] = {0xFFC4, 0xFFCA, 0xFFCA, 0xFFDC, 0xFFC5};

    uint16_t average_data = get_average_data_in_array(data_array, 5);

    TEST_ASSERT_EQUAL(0xFFCB, average_data);
}

extern uint16_t Gl, Vl, Vu;

void test_threshold()
{
    Gl = 24;
    Vl = 1;
    Vu = 250;

    const uint8_t Ks = 4; // 4 per mille = 0.4%
    const uint16_t Thr = get_threshold(Ks) ;

    TEST_ASSERT_EQUAL_UINT16 (0x08BD, Thr);
}

void test_offset()
{
    uint16_t measured_value = 0x2710;
    uint16_t gain = 0x5AA7;
    uint16_t expected_offset = 906;
    uint16_t actual_offset =  get_offset_from_measured(measured_value, gain);

    TEST_ASSERT_EQUAL_UINT16(expected_offset, actual_offset);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_tx_data);
    RUN_TEST(test_conv_uint32_to_uint8_array);
    RUN_TEST(test_get_mmode_value);
    RUN_TEST(test_average_data);
    RUN_TEST(test_threshold);
    RUN_TEST(test_offset);

    UNITY_END();
}