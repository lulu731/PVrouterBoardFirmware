#include "uri_handlers.h"

#include "adc_registers.h"
#include "adc_rw.h"
#include "calibration_params.h"
#include "json.h"

#include "esp_log.h"
#include "esp_http_server.h"

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

char* calibration_file ="/littlefs/calibration.html";
unsigned char* ws_payload; // caller is responsible to free

static esp_err_t get_ws_payload(httpd_req_t *req, unsigned char* payload)
{
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    ws_payload = NULL;

    /* Set max_len = 0 to get the frame len */
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }
    ESP_LOGI(TAG, "frame len is %d", ws_pkt.len);

    if (ws_pkt.len) {
        /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
        ws_payload = calloc(1, ws_pkt.len + 1);
        if (ws_payload == NULL) {
            ESP_LOGE(TAG, "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = ws_payload;
        /* Set max_len = ws_pkt.len to get the frame payload */
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(ws_payload);
            return ret;
        }
        ESP_LOGI(TAG, "Got packet with message: %s", ws_pkt.payload);
    }
    return ESP_OK;
}

esp_err_t calibration_handler(httpd_req_t *req)
{
    esp_err_t err = ESP_OK;
    if (req->method == HTTP_GET)
    {
        err = handler_first_call(req, calibration_file);
        return err;
    }

    get_ws_payload(req, ws_payload);

    return ESP_OK;
}

extern uint16_t Ugain;
extern uint16_t IgainL;
extern uint16_t IgainN;

esp_err_t ws_handler(httpd_req_t *req)
{
    get_ws_payload(req, ws_payload);

    if (ws_payload) {
        // Parse JSON to get the ID and value
        gain_object* obj = json_parse_gain_object((const char*)ws_payload);

        if (obj != NULL && obj->key != NULL) {
            // Update the appropriate variable based on the ID
            if (strcmp(obj->key, "Ugain") == 0) {
                Ugain = obj->value;
                const struct adc_register reg = { .address = 0x23, .data = Ugain };
                write_adc_register(reg);
            }
            else if (strcmp(obj->key, "IgainL") == 0) {
                IgainL = obj->value;
                const struct adc_register reg = { .address = 0x23, .data = IgainL };
                write_adc_register(reg);
            }
            else if (strcmp(obj->key, "IgainN") == 0) {
                IgainN = obj->value;
                const struct adc_register reg = { .address = 0x23, .data = IgainN };
                write_adc_register(reg);
            }

            // Free the allocated memory
            if (obj != &null_gain_object) {
                free(obj->key);
                free(obj);
            }
        }

        free(ws_payload);
        ws_payload = NULL;
    }

    return ESP_OK;
}
