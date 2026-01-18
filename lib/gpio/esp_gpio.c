#include "esp_gpio.h"

#include "driver/gpio.h"

/**
 * @brief Initialize GPIO configuration for a given GPIO number
 *
 * This function will set the GPIO mode to output and disable interrupts, pull-up and pull-down.
 * It will then set the GPIO level to 0.
 *
 * @param gpio_num GPIO number to be configured
 */
void init_gpio_config_for_gpio(gpio_num_t gpio_num)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1ULL<<gpio_num);
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    gpio_set_level(gpio_num, 0);
}