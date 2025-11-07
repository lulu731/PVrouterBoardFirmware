#include "driver/gpio.h"

#include "unity.h"

#include "esp_err.h"

int calls_to_set_level_h = 0;
int calls_to_set_level_l = 0;

static bool is_low_level = true;

esp_err_t gpio_config(const gpio_config_t *pGPIOConfig)
{
    return ESP_OK;
}

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level)
{
    TEST_ASSERT_TRUE(gpio_num == GPIO_NUM_18);
    if (level == 1)
        calls_to_set_level_h++;
    else
        calls_to_set_level_l++;
    is_low_level = !(bool)level;
    return ESP_OK;
}

bool level_is_low(void)
{
    return is_low_level;
}
