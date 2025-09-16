
#ifdef TEST

#include "unity.h"

#include "mock_mount_partition_Hardware.h"
#include "mount_partition_Driver.h"

static const partition_config_t conf = {
    .base_path = "/mount_point",
    .partition_label = "partition_name"
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mount_partition_Driver_create(void)
{
    const fs_config_t fs_config =
    {
        .base_path = conf.base_path,
        .partition_label = conf.partition_label
    };
    fs_init_Expect(&fs_config);
    mount_part_create(&conf);
}

void test_mount_partition_Driver(void)
{
    fs_mount_ExpectAndReturn(HARD_MOUNT_OK);
    mount_error_t error = mount_part();

    TEST_ASSERT_EQUAL(MOUNT_OK, error);
}

void test_mount_partition_Driver_error(void)
{
    fs_mount_ExpectAndReturn(HARD_MOUNT_ERROR);
    mount_error_t error = mount_part();

    TEST_ASSERT_EQUAL(MOUNT_ERROR, error);
}

#endif // TEST
