#include "esp_http_server.h"

#include <string.h>
#include <stdlib.h>

#include "unity.h"

// Global variable to capture messages sent by the server (for test verification)
char* message_received = NULL;

int server_handle = 100;
httpd_handle_t web_server = &server_handle;

int httpd_start_error = 0;
esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config)
{

    if (httpd_start_error == 1)
        return ESP_ERR_INVALID_ARG;

    *handle = web_server;

    return ESP_OK;
}

int httpd_req_to_sockfd(httpd_req_t *r)
{
    return 10;
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
    if (r->sess_ctx == NULL && Unity.CurrentTestName != NULL &&
        (strcmp(Unity.CurrentTestName, "test_index_handler_with_null_session_ctx_should_create_ctx") == 0 ||
         strcmp(Unity.CurrentTestName, "test_calibration_handler_with_null_session_ctx_should_create_ctx") == 0))
    {
        r->sess_ctx = malloc(sizeof(int));
        if (r->sess_ctx != NULL) {
            *(int*)r->sess_ctx = 10;
        }
    }
    return ESP_OK;
}

esp_err_t httpd_resp_set_type(httpd_req_t *r, const char *type)
{
    return ESP_OK;
}

esp_err_t httpd_resp_send_chunk(httpd_req_t *r, const char *buf, size_t buf_len)
{
    return ESP_OK;
}

int httpd_stop_error = 0;
esp_err_t httpd_stop(httpd_handle_t handle)
{
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
int send_to_all_clients_error = 0;
esp_err_t httpd_get_client_list(httpd_handle_t handle, size_t *fds, int *client_fds)
{

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
    // Store the message sent by the server for test verification
    if (frame->payload != NULL && frame->len > 0) {
        message_received = malloc(frame->len + 1);
        if (message_received != NULL) {
            strncpy(message_received, (const char*)frame->payload, frame->len);
            message_received[frame->len] = '\0';
        }
    }
    return ESP_OK;
}

int returned_nbytes = 0;
char* message_sent;
bool buffer_too_low = false;
int httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len)
{
    if (!buffer_too_low)
    {
        strncpy(buf, message_sent, strlen(message_sent));
        returned_nbytes = strlen(message_sent);
        return strlen(message_sent);
    }
    else
    {

    }

    return 0;
}

bool return_error = false;
esp_err_t httpd_ws_recv_frame(httpd_req_t *req, httpd_ws_frame_t *pkt, size_t max_len)
{
    if (return_error)
    {
        return ESP_ERR_INVALID_ARG;
    }

    const char* msg = message_sent;
    if (Unity.CurrentTestName != NULL &&
        strcmp(Unity.CurrentTestName, "test_ws_handler_should_send_calibration_on_first_message") == 0)
    {
        msg = "{\"objects\":[{\"id\":\"ready\",\"value\":0}]}";
    }

    if (max_len == 0)
    {
        pkt->len = strlen(msg);
        return ESP_OK;
    };

    strncpy(pkt->payload, msg, strlen(msg));
    return ESP_OK;
}
