#include "server_Hardware.h"

#include <stdlib.h>
#include <string.h>

#include "esp_http_server.h"

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

void work_fn_arg_destroy(struct work_fn_arg *arg)
{
    free(arg->frame);
    free(arg);
}
