#ifdef TEST

#include "unity.h"

#include "system.h"
#include "adc_registers.h"


TEST_SOURCE_FILE("test/system/fake_adc_rw.c");


void setUp(void)
{
}

void tearDown(void)
{
}

extern struct adc_register P_MEAN;

void test_get_main_real_power(void)
{
    P_MEAN.data = 0b1111111100110111; /// adc read value returned in register
    int16_t main_real_power = get_main_real_power();
    TEST_ASSERT_EQUAL_INT16(-200, main_real_power);
}

void test_get_main_real_power_returns_neg300(void)
{
    P_MEAN.data = 0b1111111011010011; /// adc read value
    int16_t main_real_power = get_main_real_power();
    TEST_ASSERT_EQUAL_INT16(-300, main_real_power);
}

void test_get_main_real_power_returns_300(void)
{
    P_MEAN.data = 0b0000000100101100; /// adc read value
    int16_t main_real_power = get_main_real_power();
    TEST_ASSERT_EQUAL_INT16(300, main_real_power);
}


#endif // TEST
