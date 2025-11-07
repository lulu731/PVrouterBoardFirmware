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

TEST_SOURCE_FILE("test/app/stub_nvs.c");
TEST_SOURCE_FILE("test/app/stub_nvs_flash.c");
TEST_SOURCE_FILE("test/app/fake_mount_partition.c");
TEST_SOURCE_FILE("test/app/stub_gpio.c");
TEST_SOURCE_FILE("test/app/stub_spi_master.c");

static const int POWER_THRESHOLD = 100;
spi_device_handle_t meter_handle;

extern int nbr_access_to_adc;
int nbr_calls_to_trigger;

void setUp(void)
{
    nbr_calls_to_trigger = 0;
}

void tearDown(void)
{
}

extern struct adc_register P_MEAN;

/// adc read value, 7 values less than -100
int16_t p_main_data[] = {0b1111111100110111, 0b0000000100101100, 0b1111111011010011,
                         0b1111111100110111, 0b0000000100101100, 0b1111111011010011,
                         0b1111111100110111, 0b0000000100101100, 0b1111111011010011,
                         0b1111111100110111};

void test_app_skeleton(void)
{
    init_ADC();
    TEST_ASSERT_EQUAL_INT(7, nbr_access_to_adc);

    for (uint8_t i = 0; i < 10; i++)
    {
        P_MEAN.data = p_main_data[i];// adc read value needed for tests
        if (get_main_real_power() < -POWER_THRESHOLD)
        {
            trigger_relay();
            nbr_calls_to_trigger++;
        }
    }

    TEST_ASSERT_EQUAL_INT(7, nbr_calls_to_trigger);
}

#endif // TEST