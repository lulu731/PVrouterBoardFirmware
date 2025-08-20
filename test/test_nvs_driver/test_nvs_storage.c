#ifdef TEST

#include "unity.h"

#include "nvs_storage.h"

#include "mock_nvs.h"

const char* namespace;

void mock_nvs_open(const esp_err_t returned_nvs_error, nvs_handle_t handle) {
    nvs_open_ExpectAndReturn(namespace, NVS_READWRITE, 0, returned_nvs_error);
    nvs_open_IgnoreArg_out_handle();
    nvs_open_ReturnThruPtr_out_handle(&handle);
}

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
    mock_nvs_open(ESP_OK, 0);
    TEST_ASSERT_EQUAL_UINT8(NVS_STORAGE_OK, nvs_storage_open());
}

void test_nvs_storage_open_error(void)
{
    mock_nvs_open(ESP_FAIL, 0);
    TEST_ASSERT_EQUAL_UINT8(NVS_STORAGE_ERROR, nvs_storage_open());
}

void test_nvs_storage_close(void) {
    nvs_handle_t handle = 100;
    mock_nvs_open(ESP_OK, handle);

    nvs_err_t nvs_error = nvs_storage_open();

    nvs_close_Expect(handle);

    nvs_storage_close();
}

#endif // TEST
