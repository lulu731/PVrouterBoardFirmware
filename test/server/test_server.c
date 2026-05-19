#ifdef TEST

#include "unity.h"

#include "adc_rw.h"
#include "server.h"
#include "server_Hardware.h"
#include "uri_handlers.h"
#include "calibration_params.h"
#include "calibration_helpers.h"
#include "nvs_storage.h"
#include "adc_registers.h"
#include "driver/spi_master.h"

#include "esp_err.h"
#include "esp_http_server.h"

#include <string.h>
#include <stdlib.h>

TEST_INCLUDE_PATH("test/support/esp-idf/include/common")

// Include source files that are needed for linking
#include "json_utils.h"
#include "websocket_utils.h"
#include "client_utils.h"
#include "json_utils.h"
#include "server_Hardware.h"

// Include cJSON for JSON parsing (needed by uri_handlers.c)
#include "cJSON.h"
#include "json.h"

// Use mock for adc_rw to avoid duplicate symbol issues
#include "mock_adc_rw.h"

// Include calibration_params and adc_rw (needed by uri_handlers.c ws_handler)

spi_device_handle_t meter_handle;

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

void test_server_start_should_define_last_uri_as_script_js(void)
{
    extern int server_start_counter;
    extern const httpd_uri_t *uri;
    extern httpd_handle_t handle_param;

    server_err_t err = server_start();

    TEST_ASSERT_EQUAL_INT(*(int*)web_server, *(int*)handle_param);
    TEST_ASSERT_EQUAL_STRING("/script.js", uri->uri);
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

// Tests for create_broadcast_json_message() helper function

// Callback function to set different return values for each call
static int fake_read_adc_callback(struct adc_register* reg, int call_count)
{
    if (reg == NULL) return -1;

    // First call: U_RMS
    if (call_count == 0)
    {
        reg->data = 2300;
    }
    // Second call: I_RMS
    else if (call_count == 1)
    {
        reg->data = 1500;
    }
    // Third call: I_RMS_2
    else if (call_count == 2)
    {
        reg->data = 1200;
    }

    return 0;
}

void test_create_broadcast_json_message_should_return_non_null(void)
{
    // Use callback to set return values
    read_adc_register_StubWithCallback(fake_read_adc_callback);

    char* json = create_broadcast_json_message();

    TEST_ASSERT_NOT_NULL(json);

    free(json);
}

void test_create_broadcast_json_message_should_contain_correct_values(void)
{
    // Use callback to set specific values
    // The callback sets: U_RMS=2300, I_RMS=1500, I_RMS_2=1200
    read_adc_register_StubWithCallback(fake_read_adc_callback);

    char* json = create_broadcast_json_message();

    // JSON format: {"objects":[{"id":"U_RMS","value":2300},{"id":"I_RMS","value":1500},{"id":"I_RMS_2","value":1200}]}

    // Verify the JSON contains all expected key-value pairs
    TEST_ASSERT_NOT_NULL(strstr(json, "\"id\":\"U_RMS\""));
    TEST_ASSERT_NOT_NULL(strstr(json, "\"value\":2300"));

    TEST_ASSERT_NOT_NULL(strstr(json, "\"id\":\"I_RMS\""));
    TEST_ASSERT_NOT_NULL(strstr(json, "\"value\":1500"));

    TEST_ASSERT_NOT_NULL(strstr(json, "\"id\":\"I_RMS_2\""));
    TEST_ASSERT_NOT_NULL(strstr(json, "\"value\":1200"));

    free(json);
}

#endif // TEST
