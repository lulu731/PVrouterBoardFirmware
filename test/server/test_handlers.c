#ifdef TEST

#include "unity.h"

#include "uri_handlers.h"

#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"

#include <stdlib.h>
#include <string.h>

TEST_INCLUDE_PATH("test/support/include/handlers")

httpd_req_t req;
extern char* index_file;
extern char* calibration_file;

int ESP_LOGE_called;
int session_ctx;
void setUp(void)
{
    req.sess_ctx = NULL;
    session_ctx = 10;
    ESP_LOGE_called = 0;
    calibration_file = "data/calibration.html";
}

void tearDown(void)
{
    if (req.sess_ctx)
        free(req.sess_ctx);
}

void test_index_handler_with_null_session_ctx_should_create_ctx(void)
{
    esp_err_t err = index_handler(&req);

    TEST_ASSERT_NOT_NULL(req.sess_ctx);
}


void test_index_handler_with_null_session_ctx_should_return_ok(void)
{
    index_file = "data/index.html";

    esp_err_t err = index_handler(&req);

    TEST_ASSERT_EQUAL_INT(ESP_OK, err);
}

void test_index_handler_with_unexistant_file_should_fail(void)
{
    index_file = "/data/xindex.html";

    esp_err_t err = index_handler(&req);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, ESP_LOGE_called, "ESP_LOGE should be called");
    TEST_ASSERT_EQUAL_INT_MESSAGE(ESP_FAIL, err, "index_handler should return ESP_FAIL");
}

static void when_session_ctx_is_not_null(httpd_req_t *req)
{
    save_req_session_context(req);
}

void test_index_handler_with_non_null_session_ctx_should_not_create_ctx(void)
{
    when_session_ctx_is_not_null(&req);
    esp_err_t err = index_handler(&req);
    TEST_ASSERT_EQUAL_INT(session_ctx, *(int*)req.sess_ctx);
}

void test_index_handler_with_non_null_session_ctx_should_return_ok(void)
{
    when_session_ctx_is_not_null(&req);
    esp_err_t err = index_handler(&req);
    TEST_ASSERT_EQUAL_INT(ESP_OK, err);
}

void test_calibration_handler_with_null_session_ctx_should_create_ctx(void)
{
    esp_err_t err = calibration_handler(&req);
    TEST_ASSERT_NOT_NULL(req.sess_ctx);
    TEST_ASSERT_EQUAL_INT(ESP_OK, err);
}

void test_calibration_handler_with_non_null_session_ctx_should_not_create_ctx(void)
{
    when_session_ctx_is_not_null(&req);
    esp_err_t err = calibration_handler(&req);
    TEST_ASSERT_EQUAL_INT(session_ctx, *(int*)req.sess_ctx);
}


void test_calibration_handler_with_non_null_session_ctx_should_return_ok(void)
{
    when_session_ctx_is_not_null(&req);
    esp_err_t err = calibration_handler(&req);
    TEST_ASSERT_EQUAL_INT(ESP_OK, err);
}

    char* message = NULL;
void test_calibration_handler_with_not_HTTP_TEXT_should_process_WS(void)
{
    req.method = HTTP_GET;
    esp_err_t err = calibration_handler(&req);
    TEST_ASSERT_EQUAL_INT(ESP_OK, err);

    req.method = HTTP_POST;
    err = calibration_handler(&req);

    TEST_ASSERT_EQUAL_INT(ESP_OK, err);
    TEST_ASSERT_EQUAL_STRING("a message sent", message);

}
#endif // TEST