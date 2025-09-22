#ifdef TEST

#include "unity.h"
#include "mock_esp_http_server.h"
#include "mock_server_Hardware.h"

#include "server.h"

#include "esp_err.h"

static httpd_handle_t web_server = NULL;
static int server_handle = 100;

static void server_start_expectations(httpd_handle_t *server_handle, const esp_err_t start_error)
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
    server_start_expectations(&web_server, ESP_OK);

    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_start_error(void)
{
    server_start_expectations(&web_server, ESP_ERR_INVALID_ARG);
    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
}

void test_server_stop(void)
{
    server_start_expectations(&web_server, ESP_OK);
    server_err_t err = server_start();

    httpd_stop_ExpectAnyArgsAndReturn(ESP_OK);
    err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_stop_error(void)
{
    server_start_expectations(&web_server, ESP_OK);
    server_err_t err = server_start();

    httpd_stop_ExpectAnyArgsAndReturn(ESP_ERR_INVALID_ARG);
    err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
}

void test_server_stop_should_return_OK_if_server_handle_null(void)
{
    web_server = NULL;
    server_start_expectations(&web_server, ESP_OK);
    server_err_t err = server_start();

    err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_send_to_all_clients_should_return_number_of_clients(void)
{
    server_start_expectations(&web_server, ESP_OK);
    server_err_t err = server_start();

    const char* message = "test_message";

    size_t fds = 2;
    int client_fds[2] = {10, 11};
    httpd_get_client_list_ExpectAnyArgsAndReturn(ESP_OK);
    httpd_get_client_list_ReturnThruPtr_fds(&fds);
    httpd_get_client_list_ReturnArrayThruPtr_client_fds(client_fds, fds);

    //httpd_queue_work_ExpectAndReturn(web_server, NULL, &client_fds[0], ESP_OK);
    //httpd_queue_work_IgnoreArg_work();
    //httpd_ws_send_frame_async_ExpectAndReturn(web_server, client_fds[0], NULL, ESP_OK);
    //httpd_ws_send_frame_async_IgnoreArg_frame();

    //httpd_queue_work_ExpectAndReturn(web_server, NULL, &client_fds[1], ESP_OK);
    //httpd_queue_work_IgnoreArg_work();
    //httpd_ws_send_frame_async_ExpectAndReturn(web_server, client_fds[1], NULL, ESP_OK);
    //httpd_ws_send_frame_async_IgnoreArg_frame();

    httpd_queue_work_ExpectAnyArgsAndReturn(ESP_OK);
    httpd_queue_work_ExpectAnyArgsAndReturn(ESP_OK);

    size_t result = server_send_to_all_clients(message);

    TEST_ASSERT_EQUAL_UINT8(fds, result);
}

#endif // TEST
