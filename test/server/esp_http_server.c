#include "esp_http_server.h"

int server_handle = 100;
httpd_handle_t web_server = &server_handle;

esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config)
{
    extern int httpd_start_error;
    if (httpd_start_error == 1)
        return ESP_ERR_INVALID_ARG;

    *handle = web_server;

    return ESP_OK;
}

const httpd_uri_t *uri = NULL;
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

esp_err_t httpd_stop(httpd_handle_t handle)
{
    extern int httpd_stop_error;
    if (httpd_stop_error  == 1)
        return ESP_ERR_INVALID_ARG;

    return ESP_OK;
}

static int* client_fds_fake;
static int  nber_fds_fake;

void client_fds_create_stub(int nber_fds, int* client_fds)
{
    nber_fds_fake = nber_fds;
    client_fds_fake = client_fds;
}

int httpd_client_list_counter = 0;
esp_err_t httpd_get_client_list(httpd_handle_t handle, size_t *fds, int *client_fds)
{
    extern int send_to_all_clients_error;
    httpd_client_list_counter++;

    *fds = nber_fds_fake;
    client_fds = client_fds_fake;

    if (send_to_all_clients_error == 1)
    {
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

int httpd_queue_work_counter;
esp_err_t httpd_queue_work(httpd_handle_t handle, httpd_work_fn_t work_fn, void *arg)
{
    httpd_queue_work_counter++;
    return ESP_OK;
}

esp_err_t httpd_ws_send_frame_async(httpd_handle_t hd, int fd, httpd_ws_frame_t *frame)
{
    return ESP_OK;
}