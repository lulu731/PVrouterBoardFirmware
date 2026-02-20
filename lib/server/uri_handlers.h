#ifndef URI_HANDLERS_H
#define URI_HANDLERS_H

#include "esp_http_server.h"
#include "esp_err.h"

esp_err_t save_req_session_context(httpd_req_t *req);

esp_err_t index_handler(httpd_req_t *req);
esp_err_t calibration_handler(httpd_req_t *req);
esp_err_t ws_handler(httpd_req_t *req);

#endif // URI_HANDLERS_H
