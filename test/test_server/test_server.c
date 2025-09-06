#ifdef TEST

#include "unity.h"
#include "mock_esp_http_server.h"

#include "server.h"

#include "esp_err.h"

static httpd_handle_t web_server = NULL;
static int server_handle = 100;

extern httpd_uri_t index_uri;

static void mock_server(httpd_handle_t *server_handle, const esp_err_t start_error)
{
    httpd_start_ExpectAndReturn(NULL, NULL, start_error);
    httpd_start_IgnoreArg_handle();
    httpd_start_ReturnThruPtr_handle(server_handle);
    httpd_start_IgnoreArg_config();
    if (start_error == ESP_OK)
        httpd_register_uri_handler_ExpectAnyArgsAndReturn(ESP_OK);
}

void setUp(void)
{
    web_server = &server_handle;
    server_create();
}

void tearDown(void)
{
    server_destroy();
}

void test_server_start_defining_uri_as_websocket(void)
{
    mock_server(&web_server, ESP_OK);

    server_err_t err = server_start();

    TEST_ASSERT(index_uri.is_websocket);
    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_start_error(void)
{
    mock_server(&web_server, ESP_ERR_INVALID_ARG);
    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
}

void test_server_stop(void)
{
    mock_server(&web_server, ESP_OK);
    server_err_t err = server_start();

    httpd_stop_ExpectAnyArgsAndReturn(ESP_OK);
    err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_stop_error(void)
{
    mock_server(&web_server, ESP_OK);
    server_err_t err = server_start();

    httpd_stop_ExpectAnyArgsAndReturn(ESP_ERR_INVALID_ARG);
    err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
}

void test_server_stop_should_return_OK_if_server_handle_null(void)
{
    web_server = NULL;
    mock_server(&web_server, ESP_OK);
    server_err_t err = server_start();

    err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

#endif // TEST
