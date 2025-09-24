#include "esp_http_server.h"

int server_handle = 100;
httpd_handle_t web_server = &server_handle;

int start_counter = 0;
esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config)
{
    *handle = web_server;
    start_counter++;
    return ESP_OK;
}

httpd_uri_t *uri = NULL;
httpd_handle_t handle_param;
esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t *uri_handler)
{
    handle_param = handle;
    uri = uri_handler;
    return ESP_OK;
}

esp_err_t httpd_resp_send_err(httpd_req_t *req, httpd_err_code_t error, const char *msg)
{
    return ESP_OK;
}

esp_err_t httpd_resp_sendstr_chunk(httpd_req_t *r, const char *str)
{
    return ESP_OK;
}
