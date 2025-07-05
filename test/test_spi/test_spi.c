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

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_tx_data);

    UNITY_END();
}