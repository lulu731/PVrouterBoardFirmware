#include "uri_handlers.h"

#include "adc_registers.h"
#include "adc_rw.h"
#include "calibration_params.h"
#include "json.h"
#include "nvs_storage.h"

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

static const uint16_t ADC_GAIN_REG_ADDRESS = 0x23;

/**
 * @brief Saves all calibration parameters to NVS storage
 * @return ESP_OK on success, ESP_FAIL otherwise
 */
static esp_err_t save_calibration_to_nvs(void)
{
    nvs_data_t data;
    nvs_storage_create("meter_config");

    if (nvs_storage_open() != NVS_STORAGE_OK) {
        ESP_LOGE(TAG, "Failed to open NVS storage");
        return ESP_FAIL;
    }

    // Save Ugain
    data.key = "Ugain";
    data.value = Ugain;
    if (nvs_storage_save(data) != NVS_STORAGE_OK) {
        ESP_LOGE(TAG, "Failed to save Ugain to NVS");
        nvs_storage_close();
        return ESP_FAIL;
    }

    // Save IgainL
    data.key = "IgainL";
    data.value = IgainL;
    if (nvs_storage_save(data) != NVS_STORAGE_OK) {
        ESP_LOGE(TAG, "Failed to save IgainL to NVS");
        nvs_storage_close();
        return ESP_FAIL;
    }

    // Save IgainN
    data.key = "IgainN";
    data.value = IgainN;
    if (nvs_storage_save(data) != NVS_STORAGE_OK) {
        ESP_LOGE(TAG, "Failed to save IgainN to NVS");
        nvs_storage_close();
        return ESP_FAIL;
    }

    nvs_storage_close();
    ESP_LOGI(TAG, "Calibration saved to NVS");
    return ESP_OK;
}

/**
 * @brief Updates a gain value and writes it to the ADC register
 * @param key The gain parameter key ("Ugain", "IgainL", "IgainN")
 * @param value The new value to set
 * @return ESP_OK on success, ESP_FAIL otherwise
 */
static esp_err_t update_gain_value(const char* key, uint16_t value)
{
    uint16_t* gain_ptr = NULL;

    if (strcmp(key, "Ugain") == 0) {
        gain_ptr = &Ugain;
    } else if (strcmp(key, "IgainL") == 0) {
        gain_ptr = &IgainL;
    } else if (strcmp(key, "IgainN") == 0) {
        gain_ptr = &IgainN;
    } else {
        ESP_LOGW(TAG, "Unknown gain key: %s", key);
        return ESP_FAIL;
    }

    *gain_ptr = value;

    const struct adc_register reg = { .address = ADC_GAIN_REG_ADDRESS, .data = value };
    write_adc_register(reg);

    ESP_LOGI(TAG, "Updated %s to %u", key, value);
    return ESP_OK;
}

esp_err_t ws_handler(httpd_req_t *req)
{
    esp_err_t ret = get_ws_payload(req, ws_payload);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get WebSocket payload: %d", ret);
        return ret;
    }

    if (ws_payload == NULL) {
        ESP_LOGW(TAG, "No WebSocket payload received");
        return ESP_OK;
    }

    gain_object* obj = json_parse_gain_object((const char*)ws_payload);
    if (obj == NULL) {
        ESP_LOGE(TAG, "Failed to parse gain object from JSON");
        free(ws_payload);
        ws_payload = NULL;
        return ESP_FAIL;
    }

    if (obj->key != NULL) {
        // Check for save calibration command (cmd=6)
        if (strcmp(obj->key, "cmd") == 0 && obj->value == 6) {
            ESP_LOGI(TAG, "Save calibration command received");
            ret = save_calibration_to_nvs();
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "Failed to save calibration to NVS");
            }
        } else {
            ret = update_gain_value(obj->key, obj->value);
            if (ret != ESP_OK) {
                /* Unknown key is not a fatal error - log warning and continue */
                ESP_LOGW(TAG, "Unknown or invalid key: %s", obj->key);
                ret = ESP_OK;
            }
        }
    } else {
        ESP_LOGW(TAG, "Parsed gain object has NULL key");
    }

    /* Free the parsed object */
    if (obj != &null_gain_object) {
        free(obj->key);
        free(obj);
    }

    /* Free the WebSocket payload */
    free(ws_payload);
    ws_payload = NULL;

    return ret;
}
