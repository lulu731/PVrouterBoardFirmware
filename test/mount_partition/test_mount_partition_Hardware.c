
#ifdef TEST

#include "unity.h"

#include "mount_partition_Hardware.h"

#include "mock_esp_littlefs.h"

#include "esp_err.h"

const fs_config_t conf = {
    .base_path = "/mount_point",
    .partition_label = "partition_name"
};

void setUp(void)
{
    fs_init(&conf);
}

void tearDown(void)
{
}

void test_mount_partition_Hardware(void)
{
    esp_vfs_littlefs_register_ExpectAnyArgsAndReturn(ESP_OK);
    hardware_mount_error_t res = fs_mount();
    TEST_ASSERT_EQUAL_UINT8(HARD_MOUNT_OK, res);
}

void test_mount_partition_Hardware_error(void)
{
    esp_vfs_littlefs_register_ExpectAnyArgsAndReturn(ESP_ERR_NOT_FOUND);
    hardware_mount_error_t res = fs_mount();
    TEST_ASSERT_EQUAL_UINT8(HARD_MOUNT_ERROR, res);
}

#endif // TEST
