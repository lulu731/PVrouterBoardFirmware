#include "trigger_relay.h"

#include "esp_gpio.h"

#include "driver/gpio.h"
    /*vTaskDelay(1000 / portTICK_PERIOD_MS);

    gpio_set_level(GPIO_RESET_ADC, 0);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_RESET_ADC, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);*/

#define GPIO_HEATER    GPIO_NUM_18

void create_trigger_relay(void)
{
    init_gpio_config_for_gpio(GPIO_HEATER);
}

void trigger_relay(void)
{
    gpio_set_level(GPIO_HEATER, 1);

    gpio_set_level(GPIO_HEATER, 0);
}