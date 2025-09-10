#ifndef ESP_HTTP_SERVER_H
#define ESP_HTTP_SERVER_H

#include "esp_err.h"

#include <stdbool.h>

typedef void* httpd_handle_t;

/*typedef struct httpd_config {
    unsigned    task_priority;
    size_t      stack_size;
    BaseType_t  core_id;
    uint32_t    task_caps;
    uint16_t    server_port;
    uint16_t    ctrl_port;

    uint16_t    max_open_sockets;
    uint16_t    max_uri_handlers;
    uint16_t    max_resp_headers;
    uint16_t    backlog_conn;
    bool        lru_purge_enable;
    uint16_t    recv_wait_timeout;
    uint16_t    send_wait_timeout;
    void * global_user_ctx;
    httpd_free_ctx_fn_t global_user_ctx_free_fn;
    void * global_transport_ctx;
    httpd_free_ctx_fn_t global_transport_ctx_free_fn;

    bool enable_so_linger;
    int linger_timeout;
    bool keep_alive_enable;
    int keep_alive_idle;
    int keep_alive_interval;
    int keep_alive_count;
    httpd_open_func_t open_fn;
    httpd_close_func_t close_fn;
    httpd_uri_match_func_t uri_match_fn;
} httpd_config_t;*/


/*#define HTTPD_DEFAULT_CONFIG() {                        \
        .task_priority      = tskIDLE_PRIORITY+5,       \
        .stack_size         = 4096,                     \
        .core_id            = tskNO_AFFINITY,           \
        .task_caps          = (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),       \
        .server_port        = 80,                       \
        .ctrl_port          = ESP_HTTPD_DEF_CTRL_PORT,  \
        .max_open_sockets   = 7,                        \
        .max_uri_handlers   = 8,                        \
        .max_resp_headers   = 8,                        \
        .backlog_conn       = 5,                        \
        .lru_purge_enable   = false,                    \
        .recv_wait_timeout  = 5,                        \
        .send_wait_timeout  = 5,                        \
        .global_user_ctx = NULL,                        \
        .global_user_ctx_free_fn = NULL,                \
        .global_transport_ctx = NULL,                   \
        .global_transport_ctx_free_fn = NULL,           \
        .enable_so_linger = false,                      \
        .linger_timeout = 0,                            \
        .keep_alive_enable = false,                     \
        .keep_alive_idle = 0,                           \
        .keep_alive_interval = 0,                       \
        .keep_alive_count = 0,                          \
        .open_fn = NULL,                                \
        .close_fn = NULL,                               \
        .uri_match_fn = NULL                            \
}*/

typedef struct httpd_config {
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

esp_err_t httpd_resp_send_err(httpd_req_t *req, httpd_err_code_t error, const char *msg);
esp_err_t httpd_resp_sendstr_chunk(httpd_req_t *r, const char *str);
esp_err_t httpd_ws_send_frame(httpd_req_t *req, httpd_ws_frame_t *pkt);

#define vTaskDelay(x)

#endif // ESP_HTTP_SERVER_H
