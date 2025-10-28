#ifdef TEST

#include "unity.h"

#include "trigger_relay.h"
#include "esp_gpio.h"
#include "driver/gpio.h"

extern int calls_to_set_level_h;
extern int calls_to_set_level_l;

void setUp(void)
{
    create_trigger_relay();
    calls_to_set_level_h = 0;
    calls_to_set_level_l = 0;
}

void tearDown(void)
{
}

void test_trigger_relay(void)
{
    TEST_ASSERT(level_is_low());
    trigger_relay();
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_h);
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_l);
    TEST_ASSERT(level_is_low());
}

#endif // TEST
