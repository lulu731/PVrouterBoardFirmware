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

static esp_err_t index_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET)
    {
        ESP_LOGI(TAG, "handshake done");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Reading %s", "/index.html");
    FILE* pfile = fopen("/littlefs/index.html", "r");

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
			esp_err_t ret = httpd_resp_sendstr_chunk(req, line);//, HTTPD_RESP_USE_STRLEN);
			if (ret != ESP_OK) {
				ESP_LOGE(TAG, "httpd_resp_sendstr_chunk fail %d", ret);
			}
		}
		fclose(pfile);
	}
    return ESP_OK;
}

static httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
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
        err = httpd_register_uri_handler(web_server, &index_uri);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "httpd_start fail");
        return SERVER_ERROR;
    }

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

static void httpd_work_fn(void *arg)
{
    struct work_fn_arg* work_arg = (struct work_fn_arg*)arg;
    httpd_ws_send_frame_async(web_server, work_arg->sock_fd, work_arg->frame);
    work_fn_arg_destroy(work_arg);
}

size_t server_send_to_all_clients(const char* message)
{
    size_t fds = 5;
    int* client_fds = malloc(sizeof(int) * fds);
    esp_err_t err = httpd_get_client_list(web_server, &fds, client_fds);

    for (size_t index_client_fds = 0; index_client_fds < fds; index_client_fds++)
    {
        struct work_fn_arg* arg = work_fn_arg_create(*(client_fds + index_client_fds), message);
        httpd_queue_work(web_server, httpd_work_fn, arg);
    }

    free(client_fds);
    return fds;
}

void server_destroy(void)
{
    web_server = NULL;
}