#ifndef SERVER_HARDWARE_H
#define SERVER_HARDWARE_H

#include "esp_http_server.h"

struct work_fn_arg
{
    int               sock_fd;
    httpd_ws_frame_t* frame;
};

#endif // SERVER_HARDWARE_H
