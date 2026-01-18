#include "trigger_relay.h"

#include "esp_gpio.h"

#include "driver/gpio.h"
    /*vTaskDelay(1000 / portTICK_PERIOD_MS);

    gpio_set_level(GPIO_RESET_ADC, 0);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_RESET_ADC, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);*/

#define GPIO_HEATER    GPIO_NUM_18

/**
 * @brief Create a trigger relay using the given GPIO pin
 *
 * This function will initialize a trigger relay using the given GPIO pin.
 * It will set the GPIO mode to output and disable interrupts, pull-up and pull-down.
 * It will then set the GPIO level to 0.
 */
void create_trigger_relay(void)
{
    init_gpio_config_for_gpio(GPIO_HEATER);
}

/**
 * @brief Trigger the relay connected to the given GPIO pin
 *
 * This function will set the GPIO level to 1, wait for a short period of time and then set the GPIO level to 0.
 *
 * It is intended to be used as a trigger for a relay connected to a GPIO pin.
 */
void trigger_relay(void)
{
    gpio_set_level(GPIO_HEATER, 1);

    gpio_set_level(GPIO_HEATER, 0);
}