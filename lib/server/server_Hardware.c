#include "server_Hardware.h"

#include <stdlib.h>
#include <string.h>

#include "esp_http_server.h"
#include "json.h"
#include "calibration_params.h"
#include "json_utils.h"
#include "websocket_utils.h"
#include "client_utils.h"

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
    return create_json_message_3(key1, value1, key2, value2, key3, value3);
}
