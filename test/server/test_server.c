
#ifdef TEST

#include "unity.h"
#include "mock_esp_http_server.h"

#include "server.h"

#include "esp_err.h"

static httpd_handle_t web_server = NULL;
//static httpd_config_t config;

void setUp(void)
{
    server_create();
}

void tearDown(void)
{
}

void test_server_start_defining_uri(void)
{
    httpd_start_ExpectAnyArgsAndReturn(ESP_OK);
    httpd_register_uri_handler_ExpectAnyArgsAndReturn(ESP_OK);

    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_start_error(void)
{
    httpd_start_ExpectAnyArgsAndReturn(ESP_ERR_INVALID_ARG);

    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
}

#endif // TEST
