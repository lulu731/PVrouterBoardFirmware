#ifdef TEST

#include "unity.h"

#include "trigger_relay.h"
#include "esp_gpio.h"
#include "driver/gpio.h"
#include "system.h"
#include "adc_registers.h"

TEST_SOURCE_FILE("test/trigger_relay/stub_gpio.c");
TEST_SOURCE_FILE("test/system/fake_adc_rw.c");

extern int calls_to_set_level_h;
extern int calls_to_set_level_l;

extern struct adc_register P_MEAN;

void setUp(void)
{
    create_trigger_relay();
    calls_to_set_level_h = 0;
    calls_to_set_level_l = 0;
    P_MEAN.data = 0;
}

void tearDown(void)
{
}

/**
 * @brief Test that trigger_relay toggles GPIO correctly
 */
void test_trigger_relay(void)
{
    TEST_ASSERT(level_is_low());
    trigger_relay();
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_h);
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_l);
    TEST_ASSERT(level_is_low());
}

/**
 * @brief Helper function to set P_MEAN to represent a negative power value
 * The sign bit (bit 15) is 1 for negative values
 */
void set_negative_power(int16_t power)
{
    // For negative values, set the sign bit
    // Power of -150 means we need the sign bit set
    P_MEAN.data = (uint16_t)(-power);
}

/**
 * @brief Helper function to set P_MEAN to represent a positive power value
 */
void set_positive_power(int16_t power)
{
    P_MEAN.data = (uint16_t)power;
}

/**
 * @brief Test that power_isr triggers relay when flag was set from previous call
 *
 * This tests the scenario where:
 * 1. First ISR call: power < -POWER_THRSHOLD → sets flag
 * 2. Second ISR call: flag is set → triggers relay, clears flag
 */
void test_power_isr_triggers_relay_when_flag_set(void)
{
    // First call: power below threshold, should set the flag
    // -150 in two's complement with sign bit set
    set_negative_power(200);
    power_isr();

    // Relay should NOT be triggered on first call (flag just got set)
    TEST_ASSERT_EQUAL_INT(0, calls_to_set_level_h);

    // Second call: flag is set, should trigger relay
    set_positive_power(0);  // power is 0
    power_isr();

    // Relay should be triggered now
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_h);
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_l);
}

/**
 * @brief Test that power_isr does NOT set flag when power >= -POWER_THRSHOLD
 */
void test_power_isr_does_not_set_flag_when_power_above_threshold(void)
{
    // Power at threshold (exactly -100) - sign bit not set, so it's treated as positive
    set_positive_power(100);
    power_isr();

    // Next call should NOT trigger relay because flag was not set
    set_positive_power(0);
    power_isr();

    TEST_ASSERT_EQUAL_INT(0, calls_to_set_level_h);

    // Power above threshold (-50 would be 0b1111111111001100 but that's interpreted as 65548)
    // Since our implementation uses signed comparison, let's use positive values
    set_negative_power(50);
    power_isr();

    // Should still not trigger
    P_MEAN.data = 0;
    power_isr();

    TEST_ASSERT_EQUAL_INT(0, calls_to_set_level_h);
}

#endif // TEST
