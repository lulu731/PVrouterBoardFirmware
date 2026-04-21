#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include "esp_http_server.h"
#include <stddef.h>

/**
 * @brief Gets the list of connected client file descriptors
 * @param server The HTTP server handle
 * @param config The HTTP server configuration
 * @param client_fds_out Output parameter for the client FDs array
 * @return Number of connected clients, or 0 on error
 */
size_t get_connected_clients(httpd_handle_t server, httpd_config_t* config, int** client_fds_out);

/**
 * @brief Frees the client file descriptors array
 * @param client_fds The client FDs array to free
 */
void free_client_fds(int* client_fds);

#endif // CLIENT_UTILS_H