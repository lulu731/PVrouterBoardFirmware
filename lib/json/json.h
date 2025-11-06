#ifndef JSON_H
#define JSON_H

#include <stdint.h>

typedef struct gain_object
{
    const char* key;
    uint16_t value;
} gain_object;

char* json_stringify(gain_object* object, uint8_t size);

#endif // JSON_H
