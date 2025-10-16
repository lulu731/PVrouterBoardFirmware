#include "unity.h"
#include "app.h"

#include <stddef.h>

static int i;
static const int POWER_THRESHOLD = 100;
static int excess_power[10] = {0, 150, -150, -200, 0, 150, -200, 0, -150, -150};
static int relay_state[10]  = {0, 0,   1,    1,    0, 0,   1,    0, 1,    1};

void init_ADC(void)
{
    TEST_ASSERT_TRUE(1);
}

int get_excess_ADC_power()
{
    return excess_power[i];
}

void trigger_relay(void)
{
    if (i == 2 || i == 3 || i == 6  || i == 8 || i == 9)
        TEST_ASSERT_EQUAL(1, relay_state[i]);
    else
        TEST_ASSERT_EQUAL(0, relay_state[i]);
}


void run_app(void)
{
    init_ADC();
    for (i = 0; i < 10; i++)
    {
        if (get_excess_ADC_power() < -POWER_THRESHOLD)
        {
            trigger_relay();
        }
    }
}