#include <unity.h>

#include "nvs_functions.h"
#include "calibration_params.h"

#include <string.h>

static uint16_t test1, test2, test3;
static const char* key_array[3] = {"key1", "key2", "key3"};

static uint16_t *test_array[3] = {&test1, &test2, &test3};

static char key1[5] = "key1";
static uint16_t key1_value = 0x2701;

static char key2[5] = "key2";
static uint16_t key2_value = 0x2702;

static char key3[5] = "key3";
static uint16_t key3_value = 0x2703;


void setUp(void)
{
// set stuff up here
}

void tearDown(void)
{
// clean stuff up here
}

void test_load()
{
    test1 = 0;
    test2 = 0;
    test3 = 0;

    load_param(key1, key_array, key1_value, test_array);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(key1_value, test1, "failed test1");

    load_param(key2, key_array, key2_value, test_array);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(key2_value, test2, "failed test2");

    load_param(key3, key_array, key3_value, test_array);
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(key3_value, test3, "failed test3");
}

void test_load_param_key_not_found(void) {
    test1 = 0;
    test2 = 0;
    test3 = 0;
    const char *key = "key4";

    load_param(key, key_array, 0xFF, test_array);

    TEST_ASSERT_EQUAL_UINT16(0, test1);
    TEST_ASSERT_EQUAL_UINT16(0, test2);
    TEST_ASSERT_EQUAL_UINT16(0, test3);
}

void test_keys_count_in_array(void) {
    TEST_ASSERT_EQUAL_INT(14, keys_count);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_load);
    RUN_TEST(test_load_param_key_not_found);
    RUN_TEST(test_keys_count_in_array);

    UNITY_END();
}