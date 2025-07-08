#include <unity.h>

#include "spi_functions.h"
#include "adc_param.h"

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
    const adc_param data = CAL_NEEDED;
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
    uint8_t Lgain = 0b100;
    uint8_t Ngain = 0;
    uint8_t LNsel = 0;
    uint8_t DisHPF = 0;
    uint8_t Amod = 0;
    uint8_t Rmod = 0;
    uint8_t Zxcon = 0b10;
    uint8_t Pthresh = 0b10;
    uint16_t result = get_mmode_value(Lgain, Ngain, LNsel, DisHPF, Amod, Rmod, Zxcon, Pthresh);

    TEST_ASSERT(result == 0b1000000000100010);
}

void test_average_value()
{
    const uint8_t count = 5;
    const uint16_t data_array[] = {0xFFC4, 0xFFCA, 0xFFCA, 0xFFDC, 0xFFC5};

    uint16_t average_data = get_average_value(data_array, 5);

    TEST_ASSERT_EQUAL(0xFFCB, average_data);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_tx_data);
    RUN_TEST(test_conv_uint32_to_uint8_array);
    RUN_TEST(test_get_mmode_value);
    RUN_TEST(test_average_value);

    UNITY_END();
}