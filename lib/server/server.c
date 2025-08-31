#include "server.h"

#include "esp_http_server.h"

#include <stddef.h>

static httpd_handle_t web_server = NULL;
static httpd_config_t config;
static httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = NULL,
    .user_ctx  = NULL
};


void server_create(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
}

server_err_t server_start(void)
{
    httpd_start(web_server, &config);
    httpd_register_uri_handler(web_server, &index_uri);
}

/*static void server_stop(void)
{
    httpd_stop(web_server);
}

static void server_destroy(void)
{
    //httpd_stop(web_server);
}*/