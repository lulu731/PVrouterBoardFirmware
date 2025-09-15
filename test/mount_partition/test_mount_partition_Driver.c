
#ifdef TEST

#include "unity.h"

#include "mock_mount_partition_Hardware.h"
#include "mount_partition_Driver.h"

static const char* partition_name = "partition_name";
static const char* mount_point = "/mount_point";


void setUp(void)
{
}

void tearDown(void)
{
}

void test_mount_partition_Driver_create(void)
{
    fs_init_Expect(partition_name, mount_point);
    mount_part_create(partition_name, mount_point);
}

static fs_config_t conf = {
    .base_path = "/mount_point",
    .partition_label = "partition_name"
};

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
