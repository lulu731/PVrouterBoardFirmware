#ifndef SERVER_HARDWARE_H
#define SERVER_HARDWARE_H

#include "esp_http_server.h"

struct work_fn_arg
{
    int               sock_fd;
    httpd_ws_frame_t* frame;
};

struct work_fn_arg* work_fn_arg_create(int sock_fd, const char* message);
void work_fn_arg_destroy(struct work_fn_arg* arg);


#endif // SERVER_HARDWARE_H
