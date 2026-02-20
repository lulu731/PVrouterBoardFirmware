#include "server.h"
#include "server_Hardware.h"
#include "uri_handlers.h"
#include "json.h"
#include "calibration_params.h"
#include "adc_rw.h"
#include "adc_registers.h"

#include "esp_http_server.h"
#include "esp_log.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define TAG "server.c: "

static httpd_handle_t web_server;
/*
// External calibration parameters
extern uint16_t Ugain, IgainL, IgainN;

// ADC registers for gain parameters
extern struct adc_register U_GAIN, I_GAIN_L, I_GAIN_N, CS2;
*/
static httpd_config_t config = HTTPD_DEFAULT_CONFIG();

/**
 * @brief Apply calibration gains from parameters to ADC registers and save to NVS
 */
/*static void apply_calibration_gains_from_params(void)
{
    ESP_LOGI(TAG, "Applying calibration gains: Ugain=%d, IgainL=%d, IgainN=%d", Ugain, IgainL, IgainN);

    // Write gains to ADC registers
    U_GAIN.data = Ugain;
    write_adc_register(U_GAIN);

    I_GAIN_L.data = IgainL;
    write_adc_register(I_GAIN_L);

    I_GAIN_N.data = IgainN;
    write_adc_register(I_GAIN_N);

    // Update checksum register
    read_adc_register(&CS2);
    write_adc_register(CS2);

    // Save to NVS
    // Note: save_nvs_param function is not available in current includes
    // This would need to be added if NVS persistence is required
    ESP_LOGI(TAG, "Calibration gains applied to ADC registers");
}*/

static httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL,
};

static httpd_uri_t calibration_uri = {
    .uri       = "/",
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
              httpd_register_uri_handler(web_server, &ws_uri);
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
    }

    free(client_fds);
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
 * @brief Destroys the server
 *
 * This function sets the web_server handle to NULL, effectively destroying the server.
 */
void server_destroy(void)
{
    web_server = NULL;
}