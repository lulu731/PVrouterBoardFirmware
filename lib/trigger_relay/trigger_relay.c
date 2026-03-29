#include "trigger_relay.h"

#include "esp_gpio.h"
#include "system.h"

#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_intr_alloc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stddef.h>

static uint8_t relay_trigger_flag = 0;

#define GPIO_ZX     GPIO_NUM_14
#define GPIO_HEATER GPIO_NUM_18

/**
 * @brief ISR handler for GPIO pin_zx
 *
 * This function is called when pin_zx goes high (rising edge interrupt).
 * It wraps the power_isr function for use with GPIO interrupt handler.
 */
static void IRAM_ATTR zx_gpio_isr_handler(void* arg)
{
    power_isr();
}

/**
 * @brief Initialize GPIO for pin_zx (zero-cross detection)
 *
 * This function configures the GPIO pin for zero-cross detection input.
 * It sets the GPIO as input with pull-down resistor and configures
 * a rising edge interrupt to call power_isr when the pin goes high.
 */
static void init_zx_gpio(void)
{
    // Configure GPIO as input
    gpio_set_direction(GPIO_ZX, GPIO_MODE_INPUT);

    // Enable pull-down resistor to avoid floating pin
    gpio_pullup_dis(GPIO_ZX);
    gpio_pulldown_en(GPIO_ZX);

    // Configure rising edge interrupt (trigger when pin goes HIGH)
    gpio_set_intr_type(GPIO_ZX, GPIO_INTR_POSEDGE);

    // Install GPIO ISR service
    gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);

    // Add ISR handler for pin_zx
    gpio_isr_handler_add(GPIO_ZX, zx_gpio_isr_handler, NULL);
}

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
    init_zx_gpio();
}

/**
 * @brief Trigger the relay connected to the given GPIO pin
 *
 * This function will set the GPIO level to 1, wait for a short period of time and then set the GPIO level to 0.
 *
 * It is intended to be used as a trigger for a relay connected to a GPIO pin.
 */
static void trigger_relay(void)
{
    gpio_set_level(GPIO_HEATER, 1);

    // Most relays need ~5-20ms pulse width to activate
    vTaskDelay(pdMS_TO_TICKS(5));

    gpio_set_level(GPIO_HEATER, 0);
}

/**
 * @brief ISR handler for power monitoring
 *
 * This function is designed to be called from an interrupt service routine.
 * It checks if the relay should be triggered based on the previous power reading.
 * If get_main_real_power() was less than -POWER_THRSHOLD in the previous ISR call,
 * the relay will be triggered on this call.
 */
void power_isr(void)
{
    if (relay_trigger_flag)
    {
        trigger_relay();
        relay_trigger_flag = 0;
    }

    if (get_main_real_power() < -POWER_THRSHOLD)
    {
        relay_trigger_flag = 1;
    }
}
