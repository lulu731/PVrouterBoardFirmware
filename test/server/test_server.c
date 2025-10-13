#ifdef TEST

#include "unity.h"

#include "server.h"
#include "server_Hardware.h"
#include "uri_handlers.h"

#include "esp_err.h"
#include "esp_http_server.h"

#include <string.h>
#include <stdlib.h>

TEST_INCLUDE_PATH("test/support/include/common")

void setUp(void)
{
    server_create();
}

void tearDown(void)
{
    server_destroy();
}

extern int httpd_start_error;
extern int httpd_stop_error;

extern httpd_handle_t web_server;

void test_server_start_should_define_last_uri_as_websocket(void)
{
    extern int server_start_counter;
    extern const httpd_uri_t *uri;
    extern httpd_handle_t handle_param;

    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_INT(*(int*)web_server, *(int*)handle_param);

    TEST_ASSERT_EQUAL_STRING("/ws", uri->uri);
    TEST_ASSERT(uri->is_websocket == true);
    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_start_error(void)
{
    httpd_start_error = 1;
    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
    httpd_start_error = 0;
}

extern int httpd_queue_work_counter;
int client_fds_fake[5] = {11, 12, 13, 14, 15};
void setup_server_with_clients(int nber_clients)
{
    server_err_t err = server_start();
    client_fds_create_stub(nber_clients, client_fds_fake);
    httpd_queue_work_counter = 0;
}

void test_server_stop(void)
{
    setup_server_with_clients(1);

    esp_err_t err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_OK, err);
}

void test_server_stop_error(void)
{
    setup_server_with_clients(1);

    httpd_stop_error = 1;
    esp_err_t err = server_stop();

    TEST_ASSERT_EQUAL_UINT8(SERVER_ERROR, err);
    httpd_stop_error = 0;
}

const char* message = "test_message";

void test_send_to_all_clients_should_call_httpd_get_client_list_once(void)
{
    setup_server_with_clients(2);

    server_send_to_all_clients(message);

    extern int httpd_client_list_counter;
    TEST_ASSERT(httpd_client_list_counter == 1);
}

void test_send_to_all_clients_should_call_httpd_queue_work_twice(void)
{
    int nber_clients = 2;
    setup_server_with_clients(nber_clients);

    server_send_to_all_clients(message);

    TEST_ASSERT(httpd_queue_work_counter == nber_clients);
}

void test_send_to_all_clients_should_not_call_httpd_queue_work(void)
{
    int nber_clients = 0;
    setup_server_with_clients(nber_clients);

    server_send_to_all_clients(message);

    TEST_ASSERT(httpd_queue_work_counter == nber_clients);
}

extern int send_to_all_clients_error;
void test_send_to_all_clients_should_return_0_if_get_list_error(void)
{
    int nber_clients = 2;
    setup_server_with_clients(nber_clients);

    send_to_all_clients_error = 1;
    size_t fds = server_send_to_all_clients(message);

    TEST_ASSERT_EQUAL(0, fds);
    send_to_all_clients_error = 0;
}

#endif // TEST
