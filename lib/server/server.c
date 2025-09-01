#include "server.h"

#include "esp_http_server.h"

#include <stddef.h>

static httpd_handle_t web_server = NULL;
static httpd_config_t config;

static esp_err_t index_handler(httpd_req_t *req)
{
    return ESP_OK;
}

static httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
};

void server_create(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
}

server_err_t server_start(void)
{
    esp_err_t err = httpd_start(&web_server, &config);

    if (err == ESP_OK)
        err = httpd_register_uri_handler(web_server, &index_uri);

    if (err != ESP_OK)
        return SERVER_ERROR;

    return SERVER_OK;
}

server_err_t server_stop(void)
{
    if (web_server != NULL)
    {
        esp_err_t err = httpd_stop(web_server);
        if (err != ESP_OK)
            return SERVER_ERROR;
    }
    return SERVER_OK;
}

void server_destroy(void)
{
    web_server = NULL;
}