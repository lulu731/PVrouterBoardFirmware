#ifdef TEST

#include "unity.h"

#include "json.h"
#include <stdlib.h>

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
    char* json = json_stringify("null", 0);
    TEST_ASSERT_EQUAL_STRING("{\"null\":0}", json);
    free(json);
}

void test_json_print_object(void)
{
    char* json = json_stringify("UGain", 100);
    TEST_ASSERT_EQUAL_STRING("{\"UGain\":100}", json);
    free(json);
}

#endif // TEST