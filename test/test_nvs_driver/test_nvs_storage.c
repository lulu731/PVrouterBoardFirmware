#ifdef TEST

#include "unity.h"

#include "nvs_storage.h"

// Function declarations for stub control
void stub_nvs_set_flash_init_error(int error);
void stub_nvs_set_open_error(int error);

const char* namespace;

void setUp(void)
{
    namespace = "test";
    nvs_storage_create(namespace);
    // Reset stub error states
    stub_nvs_set_flash_init_error(0);
    stub_nvs_set_open_error(0);
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
    // Set flash init to fail
    stub_nvs_set_flash_init_error(1);
    TEST_ASSERT_EQUAL_UINT8(NVS_STORAGE_ERROR, nvs_storage_open());
}

void test_nvs_storage_close(void) {
    nvs_handle_t handle = 100;

    nvs_err_t nvs_error = nvs_storage_open();

    nvs_storage_close();
}

#endif // TEST
