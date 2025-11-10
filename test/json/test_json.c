#ifdef TEST

#include "unity.h"

#include "json.h"
#include <stdlib.h>
#include <string.h>

TEST_INCLUDE_PATH(".pio/libdeps/test/cJSON/")
TEST_SOURCE_FILE("cJSON.c")

void setUp(void)
{
}

void tearDown(void)
{
}

void test_json_print_null_object(void)
{
    gain_object object = {"null", 0};
    char* json = json_stringify(&object, 1);
    TEST_ASSERT_EQUAL_STRING("{\"objects\":[{\"id\":\"null\",\"value\":0}]}", json);
    free(json);
}

const char *expected_UGain_json = "{\"objects\":[{\"id\":\"UGain\",\"value\":100}]}";
void test_json_print_object(void)
{
    gain_object object = {"UGain", 100};
    char* json = json_stringify(&object, 1);
    TEST_ASSERT_EQUAL_STRING(expected_UGain_json, json);
    free(json);
}

void test_json_print_object_array(void)
{
    char expected_json[128] = "{\"objects\":[";
    strcat(expected_json, "{\"id\":\"UGain\",\"value\":100},");
    strcat(expected_json, "{\"id\":\"IGain\",\"value\":200},");
    strcat(expected_json, "{\"id\":\"VGain\",\"value\":300}]}");

    gain_object objects[3] = {{"UGain", 100}, {"IGain", 200}, {"VGain", 300}};
    char* json = json_stringify(objects, 3);
    TEST_ASSERT_EQUAL_STRING(expected_json, json);
    free(json);
}

void test_json_get_gain_object(void)
{
    char* json = expected_UGain_json;
    gain_object* object = json_parse_gain_object(json);
    TEST_ASSERT_EQUAL_STRING("UGain", object->key);
    TEST_ASSERT_EQUAL_INT(100, object->value);
    if (object != &null_gain_object)
        free(object);
}

void test_json_should_return_null_when_bad_json_string(void)
{
    char* json = "{\"objects\":[{\"id\":UGain,\"value\":100}]}";
    gain_object* object = json_parse_gain_object(json);
    TEST_ASSERT_EQUAL_STRING(NULL, object->key);
    TEST_ASSERT_EQUAL_INT(0, object->value);
    if (object != &null_gain_object)
        free(object);
}

void test_json_should_return_null_when_string_is_not_json_string(void)
{
    char* json = "{\"UGain\"}";
    gain_object* object = json_parse_gain_object(json);
    TEST_ASSERT_EQUAL_STRING(NULL, object->key);
    TEST_ASSERT_EQUAL_INT(0, object->value);
    if (object != &null_gain_object)
        free(object);
}

#endif // TEST