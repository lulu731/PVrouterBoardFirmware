#ifndef GPIO_H
#define GPIO_H

#include "esp_err.h"
#include "esp_intr_alloc.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    GPIO_NUM_NC = -1,    /*!< Use to signal not connected to S/W */
    GPIO_NUM_4 = 4,
    GPIO_NUM_18 = 18,
} gpio_num_t;

#define BIT0     0x00000001
#define BIT1     0x00000002
#define GPIO_MODE_DEF_INPUT          (BIT0)
#define GPIO_MODE_DEF_OUTPUT         (BIT1)

typedef enum {
    GPIO_MODE_INPUT = GPIO_MODE_DEF_INPUT,
    GPIO_MODE_OUTPUT = GPIO_MODE_DEF_OUTPUT,
} gpio_mode_t;

typedef enum {
    GPIO_PULLUP_DISABLE = 0x0,
    GPIO_PULLUP_ENABLE = 0x1,
} gpio_pullup_t;

typedef enum {
    GPIO_PULLDOWN_DISABLE = 0x0,
    GPIO_PULLDOWN_ENABLE = 0x1,
} gpio_pulldown_t;

typedef enum {
    GPIO_INTR_DISABLE = 0,
    GPIO_INTR_POSEDGE = 1,
    GPIO_INTR_NEGEDGE = 2,
    GPIO_INTR_ANYEDGE = 3,
    GPIO_INTR_LOW_LEVEL = 4,
    GPIO_INTR_HIGH_LEVEL = 5,
    GPIO_INTR_MAX,
} gpio_int_type_t;

typedef struct {
    uint64_t pin_bit_mask;          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
    gpio_mode_t mode;               /*!< GPIO mode: set input/output mode                     */
    gpio_pullup_t pull_up_en;       /*!< GPIO pull-up                                         */
    gpio_pulldown_t pull_down_en;   /*!< GPIO pull-down                                       */
    gpio_int_type_t intr_type;      /*!< GPIO interrupt type                                  */
} gpio_config_t;

esp_err_t gpio_config(const gpio_config_t *pGPIOConfig);

esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode);

esp_err_t gpio_pullup_dis(gpio_num_t gpio_num);

esp_err_t gpio_pulldown_en(gpio_num_t gpio_num);

esp_err_t gpio_set_intr_type(gpio_num_t gpio_num, gpio_int_type_t intr_type);

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level);

esp_err_t gpio_install_isr_service(int intr_alloc_flags);

esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, void (*isr_handler)(void*), void* args);

bool level_is_low(void);

#endif // GPIO_H
