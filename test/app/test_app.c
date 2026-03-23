#ifdef TEST

#include "unity.h"
#include "fake_mount_partition_Driver.h"

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

extern mount_error_t fake_mount_result;

void setUp(void)
{
    nbr_calls_to_trigger = 0;
    nbr_access_to_adc = 0;
    fake_mount_result = MOUNT_OK;
    // Initialize the relay before each test
    create_trigger_relay();
}

void tearDown(void)
{
    fake_mount_result = MOUNT_OK;
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
// Helper function for mount_littlefs_partition tests
// ============================================================================

static bool mount_littlefs_partition_with_error(mount_error_t error)
{
    fake_mount_result = error;
    return mount_littlefs_partition();
}

// ============================================================================
// Integration Test 1: mount_littlefs_partition - Mount the LittleFS partition (success)
// ============================================================================

void test_mount_littlefs_partition_succeeds(void)
{
    bool result = mount_littlefs_partition_with_error(MOUNT_OK);
    TEST_ASSERT_TRUE(result);
}

// ============================================================================
// Integration Test 1b: mount_littlefs_partition - Mount fails when mount_part fails
// ============================================================================

void test_mount_littlefs_partition_fails_when_mount_part_fails(void)
{
    bool result = mount_littlefs_partition_with_error(MOUNT_ERROR);
    TEST_ASSERT_FALSE(result);
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
// Integration Test 4: ADC register access tracking
// ============================================================================

void test_adc_register_access_tracking(void)
{
    nbr_access_to_adc = 0;
    int16_t power = get_main_real_power();
    TEST_ASSERT_TRUE(nbr_access_to_adc > 0);
}

#endif // TEST
