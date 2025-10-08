#include "server.h"
#include "server_Hardware.h"

#include "esp_http_server.h"
#include "esp_log.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define TAG "server.c: "

static httpd_handle_t web_server;
static httpd_config_t config = HTTPD_DEFAULT_CONFIG();

static esp_err_t load_html(const char* file, httpd_req_t *req)
{
    ESP_LOGI(TAG, "Reading %s", file);
    FILE* pfile = fopen(file, "r");
    if (pfile == NULL)
    {
        httpd_resp_send_err(req, 404, "File Not Found");
        ESP_LOGE(TAG, "fopen fail.");
        return ESP_FAIL;
    }
    else
    {
        char line[128];
        while (fgets(line, sizeof(line), pfile) != NULL) {
            esp_err_t ret = httpd_resp_sendstr_chunk(req, line);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "httpd_resp_sendstr_chunk fail %d", ret);
            }
        }
        fclose(pfile);
    }
    return httpd_resp_sendstr_chunk(req, NULL);
}

static esp_err_t save_req_session_context(httpd_req_t *req)
{
    req->sess_ctx = malloc(sizeof(int));
    if (req->sess_ctx == NULL)
    {
        ESP_LOGE(TAG, "malloc req->sess_ctx fail");
        return ESP_FAIL;
    }
    *(int*)(req->sess_ctx) = httpd_req_to_sockfd(req);
    return ESP_OK;
}

static esp_err_t handler_first_call(httpd_req_t *req, const char* file)
{
    esp_err_t err = ESP_OK;
    if (!req->sess_ctx)
    {
        err = save_req_session_context(req);
        if (err == ESP_OK)
            err = load_html(file, req);
    }
    return err;
}

static esp_err_t index_handler(httpd_req_t *req)
{
    esp_err_t err = ESP_OK;
    err = handler_first_call(req, "/littlefs/index.html");
    return err;
}

static httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL,
};


static esp_err_t calibration_handler(httpd_req_t *req)
{
    esp_err_t err = ESP_OK;
    err = handler_first_call(req, "/littlefs/calibration.html");
    return err;
}

static httpd_uri_t calibration_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = calibration_handler,
    .user_ctx  = NULL,
};

static esp_err_t ws_handler(httpd_req_t *req)
{
    return ESP_OK;
}

static httpd_uri_t ws_uri = {
    .uri       = "/ws",
    .method    = HTTP_GET,
    .handler   = ws_handler,
    .user_ctx  = NULL,
    .is_websocket = true
};


void server_create(void)
{
    ESP_LOGI(TAG, "creating server");
    web_server = NULL;
}

server_err_t server_start(void)
{
    ESP_LOGI(TAG, "starting server");
    esp_err_t err = httpd_start(&web_server, &config);

    if (err == ESP_OK)
    {
        err = httpd_register_uri_handler(web_server, &index_uri) ||
              httpd_register_uri_handler(web_server, &calibration_uri) ||
              httpd_register_uri_handler(web_server, &ws_uri);
    }

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "httpd_start fail");
        return SERVER_ERROR;
    }
    ESP_LOGI(TAG, "server started");
    return SERVER_OK;
}

static void httpd_work_fn(void *arg)
{
    struct work_fn_arg* work_arg = (struct work_fn_arg*)arg;
    httpd_ws_send_frame_async(web_server, work_arg->sock_fd, work_arg->frame);
    work_fn_arg_destroy(work_arg);
}

size_t server_send_to_all_clients(const char* message)
{
    size_t fds = config.max_open_sockets;
    int* client_fds = malloc(sizeof(int) * fds);
    if (client_fds == NULL)
    {
        ESP_LOGE(TAG, "malloc of client_fds in send_to_all_clients fail");
        return 0;
    }

    esp_err_t err = httpd_get_client_list(web_server, &fds, client_fds);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "httpd_get_client_list fail");
        free(client_fds);
        return 0;
    }

    for (size_t index_client_fds = 0; index_client_fds < fds; index_client_fds++)
    {
        struct work_fn_arg* arg = work_fn_arg_create(*(client_fds + index_client_fds), message);
        httpd_queue_work(web_server, httpd_work_fn, arg);
    }

    free(client_fds);
    return fds;
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