#ifndef ESP_HTTP_SERVER_H
#define ESP_HTTP_SERVER_H

#include "esp_err.h"

#include <stdbool.h>
#include <stddef.h>

typedef void* httpd_handle_t;

typedef struct httpd_config {
    uint16_t    max_open_sockets;
} httpd_config_t;

#define HTTPD_DEFAULT_CONFIG() { }

enum http_method
{
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_TRACE
};

typedef enum http_method httpd_method_t;

typedef struct httpd_req_t{
    int method;
    void *sess_ctx;
} httpd_req_t;

typedef struct httpd_uri_t{
    const char       *uri;
    httpd_method_t    method;
    esp_err_t (*handler)(httpd_req_t *r);
    void *user_ctx;
    bool is_websocket;
} httpd_uri_t;

#define HTTPD_WS_TYPE_TEXT 0x1
typedef struct httpd_ws_frame_t
{
    bool fragmented;
    uint8_t type;
    uint8_t* payload;
    uint16_t len;
} httpd_ws_frame_t;


esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config);
esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t *uri_handler);
esp_err_t httpd_stop(httpd_handle_t handle);

typedef int httpd_err_code_t;

int httpd_req_to_sockfd(httpd_req_t *r);
esp_err_t httpd_resp_send_err(httpd_req_t *req, httpd_err_code_t error, const char *msg);
esp_err_t httpd_resp_sendstr_chunk(httpd_req_t *r, const char *str);
esp_err_t httpd_ws_send_frame(httpd_req_t *req, httpd_ws_frame_t *pkt);

#define vTaskDelay(x)


esp_err_t httpd_get_client_list(httpd_handle_t handle, size_t *fds, int *client_fds);

typedef void (*httpd_work_fn_t)(void *arg);
esp_err_t httpd_queue_work(httpd_handle_t handle, httpd_work_fn_t work, void *arg);
esp_err_t httpd_ws_send_frame_async(httpd_handle_t hd, int fd, httpd_ws_frame_t *frame);
esp_err_t httpd_resp_send_chunk(httpd_req_t *r, const char *buf, size_t buf_len);


void client_fds_create_stub(int nber_fds, int* client_fds);

int httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len);

esp_err_t httpd_ws_recv_frame(httpd_req_t *req, httpd_ws_frame_t *pkt, size_t max_len);


#endif // ESP_HTTP_SERVER_H
