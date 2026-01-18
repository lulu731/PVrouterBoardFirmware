#include "server_Hardware.h"

#include <stdlib.h>
#include <string.h>

#include "esp_http_server.h"

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
