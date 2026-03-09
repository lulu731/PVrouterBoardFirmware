#ifdef TEST

#include "unity.h"

#include "app.h"
#include "trigger_relay.h"
#include "esp_gpio.h"

#include "driver/spi_master.h"
#include "calibration_load_params.h"
#include "calibration_params.h"
#include "calibration.h"
#include "nvs_driver.h"
#include "adc_rw.h"
#include "adc_registers.h"
#include "calibration_helpers.h"
#include "adc_functions.h"
#include "system.h"

// launching server
#include "server.h"
#include "server_Hardware.h"
#include "uri_handlers.h"
#include "json.h"
#include "cJSON.h"

#include "nvs_storage.h"

TEST_SOURCE_FILE("test/app/stub_nvs.c");
TEST_SOURCE_FILE("test/app/stub_nvs_flash.c");
TEST_SOURCE_FILE("test/app/fake_mount_partition.c");
TEST_SOURCE_FILE("test/app/stub_gpio.c");

static const int POWER_THRESHOLD = 100;
spi_device_handle_t meter_handle;

extern int nbr_access_to_adc;
int nbr_calls_to_trigger;

void setUp(void)
{
    nbr_calls_to_trigger = 0;
    nbr_access_to_adc = 0;
    // Initialize the relay before each test
    create_trigger_relay();
}

void tearDown(void)
{
}

extern struct adc_register P_MEAN;

// Test data: values that work with the get_main_real_power() conversion
// When sign bit (bit 15) is set, the function returns a negative value
// which will be less than -100 threshold
int16_t p_main_data[] = {0b1111111100110111, 0b0000000100101100, 0b1111111011010011,
                         0b1111111100110111, 0b0000000100101100, 0b1111111011010011,
                         0b1111111100110111, 0b0000000100101100, 0b1111111011010011,
                         0b1111111100110111};

// ============================================================================
// Integration Test 1: mount_littlefs_partition - Mount the LittleFS partition
// ============================================================================

void test_mount_littlefs_partition_succeeds(void)
{
    bool result = mount_littlefs_partition();
    TEST_ASSERT_TRUE(result);
}

// ============================================================================
// Integration Test 2: init_adc() - Load calibration params and run calibration
// ============================================================================

void test_init_adc_loads_calibration_params(void)
{
    bool result = init_adc();
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(nbr_access_to_adc > 0);
}

// ============================================================================
// Integration Test 3: launch_server() - Create and start the web server
// ============================================================================

void test_launch_server_creates_and_starts_server(void)
{
    server_create();
    bool result = launch_server();
    TEST_ASSERT_TRUE(result);
    server_destroy();
}

// ============================================================================
// Integration Test 4: trigger_relay_directly - Direct relay triggering test
// ============================================================================

void test_trigger_relay_directly(void)
{
    // Test direct relay triggering (without power check)
    trigger_relay();
    extern int calls_to_set_level_h;
    TEST_ASSERT_EQUAL_INT(1, calls_to_set_level_h);
}

// ============================================================================
// Integration Test 5: trigger_relay_when_power_below_threshold - Power above threshold
// ============================================================================

void test_trigger_relay_when_power_below_threshold_does_not_trigger_when_power_above(void)
{
    P_MEAN.data = 50;  // Positive power (exporting)
    int threshold = 100;
    extern int calls_to_set_level_h;
    calls_to_set_level_h = 0;
    trigger_relay_when_power_below_threshold(threshold);
    TEST_ASSERT_EQUAL_INT(0, calls_to_set_level_h);
}

// ============================================================================
// Integration Test 6: ADC register access tracking
// ============================================================================

void test_adc_register_access_tracking(void)
{
    nbr_access_to_adc = 0;
    int16_t power = get_main_real_power();
    TEST_ASSERT_TRUE(nbr_access_to_adc > 0);
}

// ============================================================================
// Integration Test 7: Full integration flow - Mount -> Init ADC -> Server -> Relay
// This is the main integration test that validates the complete app flow
// ============================================================================

void test_full_integration_flow(void)
{
    // Step 1: Mount LittleFS partition
    bool mount_result = mount_littlefs_partition();
    TEST_ASSERT_TRUE(mount_result);

    // Step 2: Initialize ADC (load calibration params)
    bool adc_result = init_adc();
    TEST_ASSERT_TRUE(adc_result);

    // Step 3: Launch web server
    server_create();
    bool server_result = launch_server();
    TEST_ASSERT_TRUE(server_result);

    // Step 4: Simulate power monitoring and relay triggering
    int trigger_count = 0;
    extern int calls_to_set_level_h;
    calls_to_set_level_h = 0;

    // Simulate 10 power readings - use raw values where sign bit indicates negative
    for (uint8_t i = 0; i < 10; i++)
    {
        // p_main_data[i] contains raw register values
        // When sign bit (bit 15) is set, it's interpreted as negative power
        P_MEAN.data = p_main_data[i];
        if (get_main_real_power() < -POWER_THRESHOLD)
        {
            trigger_relay_when_power_below_threshold(POWER_THRESHOLD);
            trigger_count++;
        }
    }

    // Verify that relay was triggered for negative power values
    // 7 out of 10 values have sign bit set (negative power)
    TEST_ASSERT_EQUAL_INT(7, trigger_count);
    TEST_ASSERT_EQUAL_INT(7, calls_to_set_level_h);

    server_destroy();
}

#endif // TEST
