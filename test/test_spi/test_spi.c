#include <unity.h>

#include "spi_functions.h"

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
    uint16_t data = 0x5678;
    uint8_t tx_data[4];
    uint8_t d[2] = {0x56, 0x78};

    set_txdata_with(tx_data, data);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(d, tx_data, 2);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_tx_data);

    UNITY_END();
}