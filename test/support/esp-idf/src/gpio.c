#include "driver/gpio.h"

#include "unity.h"

#include "esp_err.h"
#include "esp_intr_alloc.h"

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

esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)
{
    return ESP_OK;
}

esp_err_t gpio_pullup_dis(gpio_num_t gpio_num)
{
    return ESP_OK;
}

esp_err_t gpio_pulldown_en(gpio_num_t gpio_num)
{
    return ESP_OK;
}

esp_err_t gpio_set_intr_type(gpio_num_t gpio_num, gpio_int_type_t intr_type)
{
    return ESP_OK;
}

esp_err_t gpio_install_isr_service(int intr_alloc_flags)
{
    return ESP_OK;
}

esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, void (*isr_handler)(void*), void* args)
{
    return ESP_OK;
}
