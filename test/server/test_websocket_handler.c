#ifdef TEST

#include "unity.h"

#include "uri_handlers.h"
#include "calibration_params.h"

#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"

#include <stdlib.h>
#include <string.h>

//#include "json.h"
//#include "calibration_params.h"

void setUp(void)
{
}

void tearDown(void)
{
}

// Mock variables for WebSocket testing
static int httpd_ws_recv_frame_call_count = 0;
static httpd_ws_frame_t* last_ws_frame = NULL;
static esp_err_t httpd_ws_recv_frame_return_value = ESP_OK;
/*
// Mock function for httpd_ws_recv_frame
esp_err_t httpd_ws_recv_frame(httpd_req_t *req, httpd_ws_frame_t *pkt, size_t max_len)
{
    httpd_ws_recv_frame_call_count++;
    if (last_ws_frame != NULL) {
        pkt->len = last_ws_frame->len;
        pkt->payload = last_ws_frame->payload;
        pkt->type = last_ws_frame->type;
        pkt->fragmented = last_ws_frame->fragmented;
    }
    return httpd_ws_recv_frame_return_value;
}*/

// Helper function to setup WebSocket frame for testing
void setup_ws_frame(const char* payload)
{
    if (last_ws_frame == NULL) {
        last_ws_frame = malloc(sizeof(httpd_ws_frame_t));
    }
    last_ws_frame->len = strlen(payload);
    last_ws_frame->payload = (uint8_t*)payload;
    last_ws_frame->type = HTTPD_WS_TYPE_TEXT;
    last_ws_frame->fragmented = false;
    httpd_ws_recv_frame_call_count = 0;
    httpd_ws_recv_frame_return_value = ESP_OK;
}


// WebSocket handler tests
//extern uint16_t Ugain, IgainL, IgainN;

// Mock function declarations for ADC operations (to avoid linking issues in tests)
//int write_adc_register(const struct adc_register reg) { return 0; }
//int read_adc_register(struct adc_register* reg) { return 0; }

void test_ws_handler_should_update_Ugain_parameter(void)
{
    // Setup initial values
    Ugain = 1000;

    // Setup WebSocket frame with Ugain update
    const char* json_payload = "{\"objects\":[{\"id\":\"Ugain\",\"value\":1500}]}";
    setup_ws_frame(json_payload);

    // Create a mock request
    httpd_req_t req;
    req.method = HTTP_GET;

    // Call ws_handler
    extern esp_err_t ws_handler(httpd_req_t *req);
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(1500, Ugain);  // Ugain should be updated
}

/*void test_ws_handler_should_update_IgainL_parameter(void)
{
    // Setup initial values
    Ugain = 1000;
    IgainL = 2000;
    IgainN = 3000;

    // Setup WebSocket frame with IgainL update
    const char* json_payload = "{\"objects\":[{\"id\":\"IgainL\",\"value\":2500}]}";
    setup_ws_frame(json_payload);

    // Create a mock request
    httpd_req_t req;
    req.method = HTTP_GET;

    // Call ws_handler
    extern esp_err_t ws_handler(httpd_req_t *req);
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(1000, Ugain);  // Ugain should remain unchanged
    TEST_ASSERT_EQUAL(2500, IgainL); // IgainL should be updated
    TEST_ASSERT_EQUAL(3000, IgainN); // IgainN should remain unchanged
}

void test_ws_handler_should_update_IgainN_parameter(void)
{
    // Setup initial values
    Ugain = 1000;
    IgainL = 2000;
    IgainN = 3000;

    // Setup WebSocket frame with IgainN update
    const char* json_payload = "{\"objects\":[{\"id\":\"IgainN\",\"value\":3500}]}";
    setup_ws_frame(json_payload);

    // Create a mock request
    httpd_req_t req;
    req.method = HTTP_GET;

    // Call ws_handler
    extern esp_err_t ws_handler(httpd_req_t *req);
    esp_err_t result = ws_handler(&req);

    // Verify results
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(1000, Ugain);  // Ugain should remain unchanged
    TEST_ASSERT_EQUAL(2000, IgainL); // IgainL should remain unchanged
    TEST_ASSERT_EQUAL(3500, IgainN); // IgainN should be updated
}

void test_ws_handler_should_handle_save_command(void)
{
    // Setup initial values
    Ugain = 1100;
    IgainL = 2100;
    IgainN = 3100;

    // Setup WebSocket frame with save command
    const char* json_payload = "{\"objects\":[{\"id\":\"cmd\",\"value\":6}]}";
    setup_ws_frame(json_payload);

    // Create a mock request
    httpd_req_t req;
    req.method = HTTP_GET;

    // Call ws_handler
    extern esp_err_t ws_handler(httpd_req_t *req);
    esp_err_t result = ws_handler(&req);

    // Verify results - command should be processed without errors
    TEST_ASSERT_EQUAL(ESP_OK, result);
    // Note: ADC register writes are mocked, so we can't verify them directly
    // but the function should complete without errors
}

void test_ws_handler_should_handle_invalid_json(void)
{
    // Setup initial values
    uint16_t original_Ugain = Ugain;
    uint16_t original_IgainL = IgainL;
    uint16_t original_IgainN = IgainN;

    // Setup WebSocket frame with invalid JSON
    const char* json_payload = "invalid json";
    setup_ws_frame(json_payload);

    // Create a mock request
    httpd_req_t req;
    req.method = HTTP_GET;

    // Call ws_handler
    extern esp_err_t ws_handler(httpd_req_t *req);
    esp_err_t result = ws_handler(&req);

    // Verify results - should handle gracefully without crashing
    TEST_ASSERT_EQUAL(ESP_OK, result);
    TEST_ASSERT_EQUAL(original_Ugain, Ugain);   // Values should remain unchanged
    TEST_ASSERT_EQUAL(original_IgainL, IgainL);
    TEST_ASSERT_EQUAL(original_IgainN, IgainN);
}*/



#endif // TEST
