#include "app.h"

#include "mount_partition_Driver.h"
#include "calibration.h"
#include "calibration_load_params.h"
#include "trigger_relay.h"
#include "system.h"

#include "server.h"

#include "esp_log.h"

#include <stddef.h>
#include <stdint.h>

bool mount_littlefs_partition(void)
{
    partition_config_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs"
    };

    mount_part_create(&conf);
    if (mount_part() != MOUNT_OK) {
        ESP_LOGE("main", "mount create error");
        return false;
    }

    return true;
}

bool init_adc(void)
{
    if (load_calibration_params() != 0) {
        ESP_LOGE("main", "failed to load calibration params");
        return false;
    }

    exec_metering_calibration();
    return true;
}

bool launch_server(void)
{
    server_create();
    server_err_t err = server_start();
    if (err != SERVER_OK) {
        ESP_LOGE("main", "failed to start server");
        return false;
    }

    // Start the periodic broadcast timer
    err = server_start_periodic_broadcast();
    if (err != SERVER_OK) {
        ESP_LOGW("main", "failed to start periodic broadcast, continuing without it");
    }

    return true;
}
