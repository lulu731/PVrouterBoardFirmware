#include "app.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "spi_master.h"
#include "trigger_relay.h"

void app_main(void)
{
    // Step 1: Mount LittleFS partition
    mount_littlefs_partition();

    // Step 2: Initialize ADC (load calibration params)
    init_adc();

    // Step 3: Launch web server
    launch_server();

    // Step 4: power monitoring and relay triggering
    create_trigger_relay();
}
