#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t server_err_t;

#define SERVER_OK    0
#define SERVER_ERROR 1

void server_create(void);
server_err_t server_start(void);
server_err_t server_stop(void);
size_t server_send_to_all_clients(const char* message);
void server_destroy(void);
server_err_t server_start_periodic_broadcast(void);
server_err_t server_stop_periodic_broadcast(void);

// Helper function for creating broadcast JSON message
char* create_broadcast_json_message(void);

#endif // SERVER_H
