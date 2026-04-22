#ifdef TEST

#include "unity.h"

#include "nvs_storage.h"

// Include nvs_storage.c to get the implementation
TEST_SOURCE_FILE("lib/nvs_driver/nvs_storage.c")
// Include stub for ESP-IDF NVS functions
TEST_SOURCE_FILE("test/app/stub_nvs.c")

const char* namespace;

void setUp(void)
{
    namespace = "test";
    nvs_storage_create(namespace);
}

void tearDown(void)
{
}

void test_nvs_storage_open_(void)
{
    TEST_ASSERT_EQUAL_UINT8(NVS_STORAGE_OK, nvs_storage_open());
}

void test_nvs_storage_open_error(void)
{
    TEST_ASSERT_EQUAL_UINT8(NVS_STORAGE_ERROR, nvs_storage_open());
}

void test_nvs_storage_close(void) {
    nvs_handle_t handle = 100;

    nvs_err_t nvs_error = nvs_storage_open();

    nvs_storage_close();
}

#endif // TEST
