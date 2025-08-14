#ifdef TEST

#include "unity.h"
#include "mock_nvs.h"

#include "esp_err.h"

#include "nvs_driver.h"

#include <string.h>

static nvs_data_t nvs_datas[] =
{
    {"Mc", 1000},
    {"Un", 230},
    {"Ib", 10},
    {"Gl", 1},
    {"Vl", 167},
    {"Vu", 529}
};

void setUp(void)
{
}

void tearDown(void)
{
}

void assert_nvs_data_returned(const nvs_data_t expected_nvs_data)
{
    nvs_data_t actual_nvs_data = get_first_nvs_data();
    TEST_ASSERT_EQUAL_STRING(expected_nvs_data.key, actual_nvs_data.key);
    TEST_ASSERT_EQUAL_UINT16(expected_nvs_data.value, actual_nvs_data.value);
}

void test_get_first_nvs_data(void)
{
    uint32_t int_iterator = 0;
    nvs_iterator_t iterator = &int_iterator;
    nvs_entry_find_ExpectAnyArgsAndReturn(ESP_OK);
    nvs_entry_find_ReturnThruPtr_output_iterator(&iterator);

    nvs_entry_info_t entry_info =
    {
        .namespace_name = "meter_config",
        .type = NVS_TYPE_U16
    };
    strcpy(entry_info.key, nvs_datas[*iterator].key);
    nvs_entry_info_ExpectAndReturn(iterator, NULL, ESP_OK);
    nvs_entry_info_IgnoreArg_out_info();
    nvs_entry_info_ReturnThruPtr_out_info(&entry_info);

    uint16_t param_value = nvs_datas[*iterator].value;
    nvs_get_u16_ExpectAnyArgsAndReturn(ESP_OK);
    nvs_get_u16_IgnoreArg_out_value();
    nvs_get_u16_ReturnThruPtr_out_value(&param_value);

    assert_nvs_data_returned(nvs_datas[*iterator]);
}

void test_get_first_nvs_data_no_entry_found(void)
{
    nvs_entry_find_ExpectAnyArgsAndReturn(ESP_ERR_NVS_NOT_FOUND);
    nvs_release_iterator_ExpectAnyArgs();
    assert_nvs_data_returned((nvs_data_t){NULL, 0});
}

void test_get_first_nvs_data_invalid_arg_should_not_release_iterator(void)
{
    nvs_entry_find_ExpectAnyArgsAndReturn(ESP_ERR_INVALID_ARG);
    assert_nvs_data_returned((nvs_data_t){NULL, 0});
}

#endif // TEST
