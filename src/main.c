#include "wifi_connect.h"
#include "spi_master.h"
#include "spi_functions.h"
#include "nvs_functions.h"
#include "nvs_params.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO_HEATER    18
#define GPIO_RESET_ADC 40
#define GPIO_PIN_SEL   ((1ULL<<GPIO_HEATER) | (1ULL<<GPIO_RESET_ADC))

void app_main(void)
{
    esp_err_t nvs_error = nvs_init();
    assert(nvs_error == ESP_OK);
    nvs_handle_t nvs_handle;// = 0;
    nvs_open_file("meter_config", &nvs_handle);
    load_nvs_params(nvs_handle);
    nvs_close(nvs_handle);

    connect_to_wifi();

    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    gpio_set_level(GPIO_RESET_ADC, 0);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_RESET_ADC, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    calibrate_adc();

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}