#include "app.h"

#include "mount_partition_Driver.h"
#include "calibration.h"
#include "calibration_load_params.h"
#include "trigger_relay.h"
#include "system.h"
#include "spi_master.h"

#include "nvs_storage.h"
#include "server.h"

#include "esp_log.h"

#include <stddef.h>
#include <stdint.h>

const char* TAG = "app.c";

bool mount_littlefs_partition(void)
{
    partition_config_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs"
    };

    mount_part_create(&conf);
    if (mount_part() != MOUNT_OK) {
        ESP_LOGE(TAG, "mount create error");
        return false;
    }
    ESP_LOGI(TAG, "partitions mounted");
    return true;
}

bool init_adc(void)
{
    // Initialize NVS storage first
    nvs_storage_create("meter_config");
    if (nvs_storage_open() != NVS_STORAGE_OK) {
        ESP_LOGE(TAG, "failed to open NVS storage");
        return false;
    }

    // Load calibration params (returns count of params loaded)
    int param_count = load_calibration_params();
    ESP_LOGI(TAG, "loaded %d calibration params from NVS", param_count);

    calibrate_adc();
    //exec_metering_calibration();

    nvs_storage_close();

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
