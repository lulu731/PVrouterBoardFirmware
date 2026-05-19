#include "websocket_utils.h"
#include "esp_http_server.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a WebSocket frame
 * @param message The message to send
 * @return WebSocket frame (caller must free), or NULL on failure
 */
httpd_ws_frame_t* create_websocket_frame(const char* message)
{
    if (message == NULL) {
        return NULL;
    }

    httpd_ws_frame_t* ws_frame = malloc(sizeof(httpd_ws_frame_t));
    if (ws_frame == NULL) {
        return NULL;
    }

    ws_frame->fragmented = false;
    ws_frame->type = HTTPD_WS_TYPE_TEXT;
    ws_frame->payload = (uint8_t*)message;
    ws_frame->len = strlen(message);

    return ws_frame;
}

/**
 * @brief Frees a WebSocket frame
 * @param ws_frame The WebSocket frame to free
 */
void free_websocket_frame(httpd_ws_frame_t* ws_frame)
{
    if (ws_frame != NULL) {
        free(ws_frame);
    }
}

/**
 * @brief Creates and sends a WebSocket frame
 * @param server The HTTP server handle
 * @param sock_fd The socket file descriptor
 * @param message The message to send
 * @return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t create_and_send_ws_frame(httpd_handle_t server, int sock_fd, const char* message)
{
    if (server == NULL || message == NULL) {
        return ESP_FAIL;
    }

    httpd_ws_frame_t ws_frame = {
        .fragmented = false,
        .type = HTTPD_WS_TYPE_TEXT,
        .payload = (uint8_t*)message,
        .len = strlen(message)
    };

    esp_err_t err = httpd_ws_send_frame_async(server, sock_fd, &ws_frame);
    if (err != ESP_OK) {
        return ESP_FAIL;
    }
    return ESP_OK;
}