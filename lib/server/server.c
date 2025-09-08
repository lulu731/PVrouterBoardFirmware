#include "server.h"

#include "esp_http_server.h"
#include "esp_log.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif

#define TAG "server.c: "

static httpd_handle_t web_server;
static httpd_config_t config = HTTPD_DEFAULT_CONFIG();

static esp_err_t index_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Reading %s", "/index.html");
    FILE* pfile = fopen("/index.html", "r");

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
			/*size_t linelen = strlen(line);
			//remove EOL (CR or LF)
			for (int i=linelen;i>0;i--) {
				if (line[i-1] == 0x0a) {
					line[i-1] = 0;
				} else if (line[i-1] == 0x0d) {
					line[i-1] = 0;
				} else {
					break;
				}
			}*/
			//ESP_LOGD(TAG, "line=[%s]", line);
			//if (strlen(line) == 0) continue;
			esp_err_t ret = httpd_resp_sendstr_chunk(req, line);//, HTTPD_RESP_USE_STRLEN);
			if (ret != ESP_OK) {
				ESP_LOGE(TAG, "httpd_resp_sendstr_chunk fail %d", ret);
			}
		}
		fclose(pfile);

        int i = 0;
        while (1) {
            char data[11];
            sprintf(data, "%d", i++);

            httpd_ws_frame_t frame =
            {
                .fragmented = 0,
                .type = HTTPD_WS_TYPE_TEXT,
                .payload = (uint8_t*)data,
                .len = strlen(data)
            };
            httpd_ws_send_frame(req, &frame);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
	}
    return ESP_OK;
}

STATIC httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL,
    .is_websocket = true
};

void server_create(void)
{
    web_server = NULL;
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