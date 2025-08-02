#include <unity.h>

#include "adc_registers.h"

void setUp(void)
{
// set stuff up here
}

void tearDown(void)
{
// clean stuff up here
}

int read_adc_register(struct adc_register* reg)
{
    (*reg).data = 0xABCD;
    return 0;
};


void test_read_adc_register()
{
    struct adc_register U_OFFSET =
    {
        .address = 0x34,
        .data = 0
    };

    read_adc_register(&U_OFFSET);
    TEST_ASSERT(U_OFFSET.data == 0xABCD);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_read_adc_register);

    UNITY_END();
}