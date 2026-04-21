#ifndef SERVER_HARDWARE_H
#define SERVER_HARDWARE_H

#include "esp_http_server.h"
#include "server.h"

struct work_fn_arg
{
    int               sock_fd;
    httpd_ws_frame_t* frame;
};

struct work_fn_arg* work_fn_arg_create(int sock_fd, const char* message);
void work_fn_arg_destroy(struct work_fn_arg* arg);

// Helper function to create and send WebSocket frames
esp_err_t create_and_send_ws_frame(httpd_handle_t server, int sock_fd, const char* message);

// Helper function to get connected client list
size_t get_connected_clients(httpd_handle_t server, httpd_config_t* config, int** client_fds_out);

// Helper function to create gain JSON messages
char* create_gain_json_message(const char* key1, uint16_t value1,
                             const char* key2, uint16_t value2,
                             const char* key3, uint16_t value3);


#endif // SERVER_HARDWARE_H
