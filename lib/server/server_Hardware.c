#include "server_Hardware.h"

#include <stdlib.h>
#include <string.h>

#include "esp_http_server.h"
#include "json.h"
#include "calibration_params.h"

/**
 * @brief Creates a work_fn_arg struct to be passed to httpd_queue_work.
 * @param sock_fd The socket file descriptor of the client.
 * @param message The message to be sent to the client.
 * @return A pointer to a work_fn_arg struct.
 */
struct work_fn_arg *work_fn_arg_create(int sock_fd, const char* message)
{
    struct work_fn_arg* arg = malloc(sizeof(struct work_fn_arg));
    arg->frame = malloc(sizeof(httpd_ws_frame_t));
    arg->frame->fragmented = 0;
    arg->frame->type = HTTPD_WS_TYPE_TEXT;
    arg->frame->payload = (uint8_t*)message;
    arg->frame->len = strlen(message);
    arg->sock_fd = sock_fd;

    return arg;
}

/**
 * @brief Frees the memory allocated for a work_fn_arg struct.
 * @param arg A pointer to the work_fn_arg struct to be freed.
 */
void work_fn_arg_destroy(struct work_fn_arg *arg)
{
    free(arg->frame);
    free(arg);
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

/**
 * @brief Gets the list of connected client file descriptors
 * @param server The HTTP server handle
 * @param config The HTTP server configuration
 * @param client_fds_out Output parameter for the client FDs array
 * @return Number of connected clients, or 0 on error
 */
size_t get_connected_clients(httpd_handle_t server, httpd_config_t* config, int** client_fds_out)
{
    size_t fds = config->max_open_sockets;
    int* client_fds = malloc(sizeof(int) * fds);
    if (client_fds == NULL) {
        return 0;
    }

    esp_err_t err = httpd_get_client_list(server, &fds, client_fds);
    if (err != ESP_OK) {
        free(client_fds);
        return 0;
    }

    *client_fds_out = client_fds;
    return fds;
}

/**
 * @brief Creates a JSON message from gain values
 * @param key1 First key name
 * @param value1 First value
 * @param key2 Second key name
 * @param value2 Second value
 * @param key3 Third key name
 * @param value3 Third value
 * @return JSON string (caller must free), or NULL on failure
 */
char* create_gain_json_message(const char* key1, uint16_t value1,
                             const char* key2, uint16_t value2,
                             const char* key3, uint16_t value3)
{
    gain_object objects[3] = {
        {key1, value1},
        {key2, value2},
        {key3, value3}
    };
    return json_stringify(objects, 3);
}
