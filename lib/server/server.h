#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

typedef uint8_t server_err_t;

#define SERVER_OK    0
#define SERVER_ERROR 1

void server_create(void);
server_err_t server_start(void);

#endif // SERVER_H
