#include "server.h"
#include "server_Hardware.h"
#include "uri_handlers.h"
#include "json.h"
#include "calibration_params.h"
#include "adc_rw.h"
#include "adc_registers.h"
#include "calibration_constants.h"

#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_timer.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define TAG "server.c: "

static httpd_handle_t web_server;

static httpd_config_t config = HTTPD_DEFAULT_CONFIG();
static esp_timer_handle_t periodic_broadcast_timer = NULL;

static httpd_uri_t script_uri = {
    .uri       = "/script.js",
    .method    = HTTP_GET,
    .handler   = script_handler,
    .user_ctx  = NULL,
};

static httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL,
};

static httpd_uri_t calibration_uri = {
    .uri       = "/calibration.html",
    .method    = HTTP_GET,
    .handler   = calibration_handler,
    .user_ctx  = NULL,
};

static httpd_uri_t ws_uri = {
    .uri       = "/ws",
    .method    = HTTP_GET,
    .handler   = ws_handler,
    .user_ctx  = NULL,
    .is_websocket = true
};

/**
 * @brief Initialize the server
 *
 * This function initializes the server by setting the web_server handle to NULL.
 *
 */
void server_create(void)
{
    ESP_LOGI(TAG, "creating server");
    config.server_port = 8081;
    web_server = NULL;
}

/**
 * @brief Start the server
 *
 * This function starts the server by calling httpd_start and registering three URI handlers.
 * The URI handlers are for the index, calibration and websocket endpoints.
 * If any of the operations fail, an error is logged and SERVER_ERROR is returned.
 * If all operations succeed, SERVER_OK is returned.
 *
 * @return SERVER_OK if the server was started successfully, SERVER_ERROR otherwise
 */
server_err_t server_start(void)
{
    ESP_LOGI(TAG, "starting server");
    esp_err_t err = httpd_start(&web_server, &config);

    if (err == ESP_OK)
    {
        err = httpd_register_uri_handler(web_server, &index_uri) ||
              httpd_register_uri_handler(web_server, &calibration_uri) ||
              httpd_register_uri_handler(web_server, &ws_uri) ||
              httpd_register_uri_handler(web_server, &script_uri);
    }

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "httpd_start fail");
        return SERVER_ERROR;
    }
    ESP_LOGI(TAG, "server started");
    return SERVER_OK;
}

/**
 * @brief Send a websocket frame to a client
 *
 * This function sends a websocket frame to a client by calling httpd_ws_send_frame_async.
 * It is intended to be used as a work function for httpd_queue_work.
 *
 * @param arg A pointer to a work_fn_arg struct containing the websocket frame to be sent
 */
static void httpd_work_fn(void *arg)
{
    struct work_fn_arg* work_arg = (struct work_fn_arg*)arg;
    httpd_ws_send_frame_async(web_server, work_arg->sock_fd, work_arg->frame);
    work_fn_arg_destroy(work_arg);
}

/**
 * @brief Send a message to all connected clients
 *
 * This function sends a message to all connected clients by calling httpd_get_client_list to get the list of client file descriptors, then iterating over the list and calling httpd_queue_work to send the message to each client.
 *
 * @param message The message to be sent to all connected clients
 * @return The number of clients the message was sent to
 */
size_t server_send_to_all_clients(const char* message)
{
    ESP_LOGI(TAG, "entering server_send_to_all_clients");
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
        ESP_LOGI(TAG, "queuing socket %d", index_client_fds);
    }

    free(client_fds);

    ESP_LOGI(TAG, "exiting server_send_to_all_clients");

    return fds;
}

/**
 * @brief Stop the HTTP server
 *
 * This function stops the HTTP server by calling httpd_stop with the web_server handle.
 * If httpd_stop returns an error, this function returns SERVER_ERROR.
 * Otherwise, it returns SERVER_OK.
 */
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

/**
 * @brief Create JSON message with current ADC RMS values
 *
 * This function reads the U_RMS, I_RMS, and I_RMS_2 ADC registers
 * and creates a JSON string with their values.
 *
 * @return JSON string (caller must free with free()), or NULL on failure
 */
char* create_broadcast_json_message(void)
{
    // Read ADC registers for RMS values
    read_adc_register(&U_RMS);
    read_adc_register(&I_RMS);
    read_adc_register(&I_RMS_2);

    // Create JSON with RMS values
    gain_object objects[3] = {
        {"U_RMS", U_RMS.data},
        {"I_RMS", I_RMS.data},
        {"I_RMS_2", I_RMS_2.data}
    };

    return json_stringify(objects, 3);
}

/**
 * @brief Periodic timer callback to broadcast JSON to all connected WebSocket clients
 *
 * This function is called every 2 seconds. It checks if any clients are connected
 * before sending the JSON message to avoid unnecessary work when no clients are present.
 */
static void periodic_broadcast_callback(void* arg)
{
    (void)arg;

    // Check if there are any connected clients
    size_t fds = config.max_open_sockets;
    int* client_fds = malloc(sizeof(int) * fds);
    if (client_fds == NULL)
    {
        return;
    }

    esp_err_t err = httpd_get_client_list(web_server, &fds, client_fds);
    free(client_fds);

    if (err != ESP_OK || fds == 0)
    {
        // No clients connected, don't send
        return;
    }

    // Create and send JSON message
    char* json_message = create_broadcast_json_message();
    if (json_message != NULL)
    {
        server_send_to_all_clients(json_message);
        free(json_message);
    }
}

/**
 * @brief Start the periodic broadcast timer
 *
 * Creates and starts a timer that fires every 2 seconds to broadcast
 * JSON data to all connected WebSocket clients.
 *
 * @return SERVER_OK on success, SERVER_ERROR on failure
 */
server_err_t server_start_periodic_broadcast(void)
{
    if (periodic_broadcast_timer != NULL)
    {
        ESP_LOGW(TAG, "Periodic broadcast timer already running");
        return SERVER_OK;
    }

    const esp_timer_create_args_t timer_args = {
        .callback = &periodic_broadcast_callback,
        .name = "periodic_broadcast"
    };

    esp_err_t err = esp_timer_create(&timer_args, &periodic_broadcast_timer);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create periodic broadcast timer: %d", err);
        return SERVER_ERROR;
    }

    // Start timer with broadcast interval
    err = esp_timer_start_periodic(periodic_broadcast_timer, BROADCAST_INTERVAL_US);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start periodic broadcast timer: %d", err);
        esp_timer_delete(periodic_broadcast_timer);
        periodic_broadcast_timer = NULL;
        return SERVER_ERROR;
    }

    ESP_LOGI(TAG, "Periodic broadcast timer started (2 second interval)");
    return SERVER_OK;
}

/**
 * @brief Stop the periodic broadcast timer
 *
 * Stops and deletes the periodic broadcast timer.
 *
 * @return SERVER_OK on success, SERVER_ERROR on failure
 */
server_err_t server_stop_periodic_broadcast(void)
{
    if (periodic_broadcast_timer == NULL)
    {
        ESP_LOGW(TAG, "Periodic broadcast timer not running");
        return SERVER_OK;
    }

    esp_err_t err = esp_timer_stop(periodic_broadcast_timer);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to stop periodic broadcast timer: %d", err);
        return SERVER_ERROR;
    }

    err = esp_timer_delete(periodic_broadcast_timer);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to delete periodic broadcast timer: %d", err);
        return SERVER_ERROR;
    }

    periodic_broadcast_timer = NULL;
    ESP_LOGI(TAG, "Periodic broadcast timer stopped");
    return SERVER_OK;
}

/**
 * @brief Destroys the server
 *
 * This function sets the web_server handle to NULL, effectively destroying the server.
 */
void server_destroy(void)
{
    web_server = NULL;
}

/**
 * @brief Send a message to a specific client by socket file descriptor
 *
 * This function creates a WebSocket frame and sends it to a specific client.
 *
 * @param sock_fd The socket file descriptor of the client
 * @param message The message to send
 * @return SERVER_OK on success, SERVER_ERROR on failure
 */
server_err_t server_send_to_client(int sock_fd, const char* message)
{
    if (web_server == NULL || message == NULL) {
        return SERVER_ERROR;
    }

    // Create WebSocket frame
    httpd_ws_frame_t ws_frame = {
        .fragmented = false,
        .type = HTTPD_WS_TYPE_TEXT,
        .payload = (uint8_t*)message,
        .len = strlen(message)
    };

    // Send the frame
    esp_err_t err = httpd_ws_send_frame_async(web_server, sock_fd, &ws_frame);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send WebSocket frame to client %d: %d", sock_fd, err);
        return SERVER_ERROR;
    }

    ESP_LOGI(TAG, "Sent message to client %d", sock_fd);
    return SERVER_OK;
}
