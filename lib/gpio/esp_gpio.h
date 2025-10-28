#ifndef ESP_GPIO_H
#define ESP_GPIO_H

#include "driver/gpio.h"

#include <stdint.h>

void init_gpio_config_for_gpio(gpio_num_t gpio_num);

#endif // ESP_GPIO_H
