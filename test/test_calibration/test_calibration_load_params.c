#ifdef TEST

#include "unity.h"

#include "calibration_load_params.h"
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


static uint16_t test1, test2, test3;
static const char* key_array[3] = {"key1", "key2", "key3"};

static uint16_t *test_array[3] = {&test1, &test2, &test3};

static char key1[5] = "key1";
static uint16_t key1_value = 0x2701;

static char key2[5] = "key2";
static uint16_t key2_value = 0x2702;

static char key3[5] = "key3";
static uint16_t key3_value = 0x2703;

static int expected_keys_count = 17;

void test_load()
{
    test1 = 0;
    test2 = 0;
    test3 = 0;

    load_param(key1, key_array, key1_value, test_array, 3);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(key1_value, test1, "failed test1");

    load_param(key2, key_array, key2_value, test_array, 3);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(key2_value, test2, "failed test2");

    load_param(key3, key_array, key3_value, test_array, 3);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(key3_value, test3, "failed test3");
}

void test_load_param_key_not_found(void) {
    test1 = 0;
    test2 = 0;
    test3 = 0;
    const char *key = "key4";

    load_param(key, key_array, 0xFF, test_array, 3);

    TEST_ASSERT_EQUAL_UINT16(0, test1);
    TEST_ASSERT_EQUAL_UINT16(0, test2);
    TEST_ASSERT_EQUAL_UINT16(0, test3);
}

extern const int keys_count;

void test_keys_count_in_array(void) {
    TEST_ASSERT_EQUAL_INT(expected_keys_count, keys_count);
}


#endif // TEST