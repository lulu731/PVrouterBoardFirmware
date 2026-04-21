#ifndef WEBSOCKET_UTILS_H
#define WEBSOCKET_UTILS_H

#include "esp_http_server.h"
#include "esp_err.h"

/**
 * @brief Creates a WebSocket frame
 * @param message The message to send
 * @return WebSocket frame (caller must free), or NULL on failure
 */
httpd_ws_frame_t* create_websocket_frame(const char* message);

/**
 * @brief Frees a WebSocket frame
 * @param ws_frame The WebSocket frame to free
 */
void free_websocket_frame(httpd_ws_frame_t* ws_frame);

/**
 * @brief Creates and sends a WebSocket frame
 * @param server The HTTP server handle
 * @param sock_fd The socket file descriptor
 * @param message The message to send
 * @return ESP_OK on success, ESP_FAIL otherwise
 */
esp_err_t create_and_send_ws_frame(httpd_handle_t server, int sock_fd, const char* message);

#endif // WEBSOCKET_UTILS_H
