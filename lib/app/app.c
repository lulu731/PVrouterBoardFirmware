#include "app.h"

#include "mount_partition_Driver.h"
#include "calibration.h"
#include "calibration_load_params.h"
#include "trigger_relay.h"
#include "system.h"

#include "esp_log.h"

#include <stddef.h>
#include <stdint.h>

void mount_littlefs_partition(void)
{
    partition_config_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs"
    };

    mount_part_create(&conf);
    if (mount_part() != MOUNT_OK) {
        ESP_LOGE("main", "mount create error");
    };

}

void init_ADC(void)
{
    load_calibration_params();
    exec_metering_calibration();
}

void trigger_relay_when_power_below_threshold(const int threshold)
{
    if (get_main_real_power() < -threshold)
    {
        trigger_relay();
    }
}
