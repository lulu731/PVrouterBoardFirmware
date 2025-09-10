
#ifdef TEST

#include "unity.h"

#include "mount_partition_Hardware.h"

#include "mock_esp_littlefs.h"

#include "esp_err.h"

void setUp(void)
{
    init_fs("partition_name", "/mount_point");
}

void tearDown(void)
{
}

void test_mount_partition_Hardware(void)
{
    esp_vfs_littlefs_register_ExpectAnyArgsAndReturn(ESP_OK);
    hardware_mount_error_t res = mount_fs();
    TEST_ASSERT_EQUAL_UINT8(HARD_MOUNT_OK, res);
}

void test_mount_partition_Hardware_error(void)
{
    esp_vfs_littlefs_register_ExpectAnyArgsAndReturn(ESP_ERR_NOT_FOUND);
    hardware_mount_error_t res = mount_fs();
    TEST_ASSERT_EQUAL_UINT8(HARD_MOUNT_ERROR, res);
}

#endif // TEST
