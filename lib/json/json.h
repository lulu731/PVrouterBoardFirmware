#ifndef JSON_H
#define JSON_H

#include <stdint.h>

typedef struct gain_object
{
    char* key;
    uint16_t value;
} gain_object;

char* json_stringify(gain_object* object, uint8_t size);
gain_object* json_parse_gain_object(const char* json_string);

#endif // JSON_H
