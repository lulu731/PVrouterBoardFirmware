#ifdef TEST

#include "unity.h"

#include "nvs_storage.h"

// Include source files that are needed for linking
#include "json_utils.h"
#include "websocket_utils.h"
#include "calibration_helpers.h"
#include "adc_registers.h"

#include "uri_handlers.h"
#include "calibration_params.h"
#include "adc_rw.h"

#include "calibration_helpers.h"

#include "driver/spi_master.h"


#include "json.h"

#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"

#include <stdlib.h>
#include <string.h>

// Include cJSON for JSON parsing
#include "cJSON.h"
#include "json.h"

spi_device_handle_t meter_handle;


// ============================================================================
// Test Setup and Teardown
// ============================================================================

// External variables from esp_http_server.c stub
extern char* message_sent;
extern int write_adc_register_call_count;
extern int write_adc_caller;

void setUp(void)
{
    write_adc_caller = 1;
    // Reset ADC write counter
    write_adc_register_call_count = 0;

    // Initialize calibration parameters to known values
    Ugain = 1000;
    IgainL = 2000;
    IgainN = 3000;
}

void tearDown(void)
{
    // Reset message_sent to avoid stale data
    message_sent = NULL;
}

// ============================================================================
// WebSocket Handler Tests
// ============================================================================

void test_ws_handler_should_update_Ugain_parameter(void)
{
    // Setup WebSocket frame with Ugain update
    message_sent = "{\"objects\":[{\"id\":\"Ugain\",\"value\":1500}]}";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(1500, Ugain);
    TEST_ASSERT_EQUAL(1, write_adc_register_call_count);
}

void test_ws_handler_should_update_IgainL_parameter(void)
{
    // Setup WebSocket frame with IgainL update
    message_sent = "{\"objects\":[{\"id\":\"IgainL\",\"value\":2500}]}";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(2500, IgainL);
    TEST_ASSERT_EQUAL(1, write_adc_register_call_count);
}

void test_ws_handler_should_update_IgainN_parameter(void)
{
    // Setup WebSocket frame with IgainN update
    message_sent = "{\"objects\":[{\"id\":\"IgainN\",\"value\":3500}]}";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(3500, IgainN);
    TEST_ASSERT_EQUAL(1, write_adc_register_call_count);
}

void test_ws_handler_should_handle_invalid_json(void)
{
    // Store original values
    uint16_t original_Ugain = Ugain;
    uint16_t original_IgainL = IgainL;
    uint16_t original_IgainN = IgainN;

    // Setup WebSocket frame with invalid JSON
    message_sent = "invalid json content";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler - should handle gracefully
    esp_err_t result = ws_handler(&req);

    // Verify results - should complete without crashing
    TEST_ASSERT_EQUAL(ESP_OK, result);
    // Values should remain unchanged
    TEST_ASSERT_EQUAL(original_Ugain, Ugain);
    TEST_ASSERT_EQUAL(original_IgainL, IgainL);
    TEST_ASSERT_EQUAL(original_IgainN, IgainN);
    // No ADC register should be written
    TEST_ASSERT_EQUAL(0, write_adc_register_call_count);
}

void test_ws_handler_should_handle_unknown_parameter(void)
{
    // Store original values
    uint16_t original_Ugain = Ugain;
    uint16_t original_IgainL = IgainL;
    uint16_t original_IgainN = IgainN;

    // Setup WebSocket frame with unknown parameter ID
    message_sent = "{\"objects\":[{\"id\":\"UnknownParam\",\"value\":9999}]}";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    // Values should remain unchanged
    TEST_ASSERT_EQUAL(original_Ugain, Ugain);
    TEST_ASSERT_EQUAL(original_IgainL, IgainL);
    TEST_ASSERT_EQUAL(original_IgainN, IgainN);
    // No ADC register should be written for unknown parameter
    TEST_ASSERT_EQUAL(0, write_adc_register_call_count);
}

void test_ws_handler_should_handle_empty_payload(void)
{
    // Store original values
    uint16_t original_Ugain = Ugain;
    uint16_t original_IgainL = IgainL;
    uint16_t original_IgainN = IgainN;

    // Setup empty WebSocket frame
    message_sent = "";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results - should handle gracefully
    TEST_ASSERT_EQUAL(ESP_OK, result);
    // Values should remain unchanged
    TEST_ASSERT_EQUAL(original_Ugain, Ugain);
    TEST_ASSERT_EQUAL(original_IgainL, IgainL);
    TEST_ASSERT_EQUAL(original_IgainN, IgainN);
}

void test_ws_handler_multiple_parameter_updates(void)
{
    // First update - Ugain
    message_sent = "{\"objects\":[{\"id\":\"Ugain\",\"value\":1111}]}";
    httpd_req_t req1;
    req1.method = HTTP_POST;
    TEST_ASSERT_EQUAL(ESP_OK, ws_handler(&req1));
    TEST_ASSERT_EQUAL(1111, Ugain);

    // Second update - IgainL
    message_sent = "{\"objects\":[{\"id\":\"IgainL\",\"value\":2222}]}";
    httpd_req_t req2;
    req2.method = HTTP_POST;
    TEST_ASSERT_EQUAL(ESP_OK, ws_handler(&req2));
    TEST_ASSERT_EQUAL(2222, IgainL);

    // Third update - IgainN
    message_sent = "{\"objects\":[{\"id\":\"IgainN\",\"value\":3333}]}";
    httpd_req_t req3;
    req3.method = HTTP_POST;
    TEST_ASSERT_EQUAL(ESP_OK, ws_handler(&req3));
    TEST_ASSERT_EQUAL(3333, IgainN);

    // Total ADC writes should be 3
    TEST_ASSERT_EQUAL(3, write_adc_register_call_count);
}

// External variable from esp_http_server.c stub
extern char* message_received;

extern int nvs_open_called, nvs_commit_called, nvs_close_called;
void test_ws_handler_should_save_calibration_to_nvs(void)
{
    // First set calibration values
    Ugain = 1500;
    IgainL = 2500;
    IgainN = 3500;

    // Setup WebSocket frame with save calibration command (cmd=6)
    message_sent = "{\"objects\":[{\"id\":\"cmd\",\"value\":6}]}";

    // Create a mock request with POST method
    httpd_req_t req;
    req.method = HTTP_POST;

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL_INT(1, nvs_open_called);
    TEST_ASSERT_EQUAL_INT(3, nvs_commit_called); //each data saving calls commit
    TEST_ASSERT_EQUAL_INT(1, nvs_close_called);
    TEST_ASSERT_EQUAL(ESP_OK, result);
}

// ============================================================================
// Calibration Initialization Tests
// ============================================================================

void test_ws_handler_should_send_calibration_on_ready_message(void)
{
    // Set specific calibration values
    Ugain = 1000;
    IgainL = 2000;
    IgainN = 3000;

    // Reset message_received
    if (message_received != NULL) {
        free(message_received);
        message_received = NULL;
    }

    // Setup WebSocket frame with any command (simulating first message from new client)
    message_sent = "{\"objects\":[{\"id\":\"ready\",\"value\":0}]}";

    // Create a mock request with POST method and NO session context (new client)
    httpd_req_t req;
    req.method = HTTP_POST;
    req.sess_ctx = NULL;  // This indicates a new client connection

    // Call ws_handler
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    // Verify that calibration values were sent back to client
    TEST_ASSERT_EQUAL_STRING_MESSAGE("{\"objects\":[{\"id\":\"Ugain\",\"value\":1000},{\"id\":\"IgainL\",\"value\":2000},{\"id\":\"IgainN\",\"value\":3000}]}",
                                    message_received, "Server should send calibration data on first message");
}

#endif // TEST
