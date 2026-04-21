#include "client_utils.h"
#include "esp_http_server.h"
#include <stdlib.h>

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
 * @brief Frees the client file descriptors array
 * @param client_fds The client FDs array to free
 */
void free_client_fds(int* client_fds)
{
    if (client_fds != NULL) {
        free(client_fds);
    }
}