#include "uri_handlers.h"

#include "esp_log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static const char TAG[] = "uri_handlers.c";

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

esp_err_t save_req_session_context(httpd_req_t *req)
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

char* index_file ="/littlefs/index.html";
esp_err_t index_handler(httpd_req_t *req)
{
    esp_err_t err = ESP_OK;
    err = handler_first_call(req, index_file);
    return err;
}

esp_err_t calibration_handler(httpd_req_t *req)
{
    esp_err_t err = ESP_OK;
    err = handler_first_call(req, "/littlefs/calibration.html");
    return err;
}
