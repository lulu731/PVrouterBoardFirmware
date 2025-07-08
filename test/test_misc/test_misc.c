#include <unity.h>

void setUp(void)
{
// set stuff up here
}

void tearDown(void)
{
// clean stuff up here
}

void test_uint8_to_uint16_data()
{
    uint16_t data;
    uint8_t data0 = 0xAB;
    uint8_t data1 = 0xCD;
    data = data0 << 8 | data1;

    TEST_ASSERT(data == 0xABCD);
}

void test_complement_value()
{
    uint16_t value = 0xFFCB;
    uint16_t complement_value = ~value + 1;

    TEST_ASSERT(complement_value == 0x35);
    TEST_ASSERT((uint16_t)(value + complement_value) == 0);
}


int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_uint8_to_uint16_data);
    RUN_TEST(test_complement_value);

    UNITY_END();
}