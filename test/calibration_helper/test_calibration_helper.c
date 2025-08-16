#ifdef TEST

#include "unity.h"

#include "calibration_helper.h"
#include "common_datas.h"

#include "mock_nvs_driver.h"

#include "calibration_params.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_load_calibration_params(void)
{
    get_first_nvs_data_ExpectAndReturn(nvs_datas[0]);
    get_next_nvs_data_ExpectAndReturn(nvs_datas[1]);
    get_next_nvs_data_ExpectAndReturn(nvs_datas[2]);
    get_next_nvs_data_ExpectAndReturn(nvs_datas[3]);
    get_next_nvs_data_ExpectAndReturn(nvs_datas[4]);
    get_next_nvs_data_ExpectAndReturn(nvs_datas[5]);
    nvs_data_t null_nvs_data = (nvs_data_t){NULL, 0};
    get_next_nvs_data_ExpectAndReturn(null_nvs_data);

    int number_of_nvs_datas = load_calibration_params();

    TEST_ASSERT_EQUAL_INT(6, number_of_nvs_datas);

    TEST_ASSERT_EQUAL_UINT16(nvs_datas[0].value, Mc);
    TEST_ASSERT_EQUAL_UINT16(nvs_datas[1].value, Un);
    TEST_ASSERT_EQUAL_UINT16(nvs_datas[2].value, Ib);
    TEST_ASSERT_EQUAL_UINT16(nvs_datas[3].value, Gl);
    TEST_ASSERT_EQUAL_UINT16(nvs_datas[4].value, Vl);
    TEST_ASSERT_EQUAL_UINT16(nvs_datas[5].value, Vu);
}

#endif // TEST