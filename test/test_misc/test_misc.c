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

void test_U_conversion()
{
    const uint16_t Ugain = 26400;
    const float_t Un = 220.024;
    const float_t Urms = 246.01;

    TEST_ASSERT_EQUAL_UINT16(23611, (uint16_t)(Ugain * Un / Urms));
}

void test_PL_constant()
{
    const uint16_t Mc = 1000;
    const uint16_t Un = 230;
    const uint16_t Ib = 10;
    const uint16_t Gl = 1;
    const uint16_t Vl = 167;
    const uint16_t Vu = 529;

    const float pl_float = 838860800 * (float)(Gl * Vl *Vu) / (float)(Mc * Un * Ib) ;
    uint32_t pl_const = pl_float;

    TEST_ASSERT_EQUAL_UINT32(0x01eba5e4,(uint32_t)(pl_float));
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_uint8_to_uint16_data);
    RUN_TEST(test_complement_value);
    RUN_TEST(test_U_conversion);
    RUN_TEST(test_PL_constant);

    UNITY_END();
}