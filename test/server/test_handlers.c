#ifdef TEST

#include "unity.h"

#include "uri_handlers.h"

#include "esp_http_server.h"
#include "esp_err.h"

#include <stdlib.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_index_handler_with_null_session_ctx_should_create_ctx(void)
{
    httpd_req_t req =
    {
        .sess_ctx = NULL
    };
    esp_err_t err = index_handler(&req);
    TEST_ASSERT_NOT_NULL(req.sess_ctx);
    free(req.sess_ctx);
}

#endif // TEST