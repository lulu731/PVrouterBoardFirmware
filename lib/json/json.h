#ifndef JSON_H
#define JSON_H

#include <stdint.h>

typedef struct gain_object
{
    char* key;
    uint16_t value;
} gain_object;

extern gain_object null_gain_object;

char* json_stringify(gain_object* gain_object, uint8_t size);
gain_object* json_parse_gain_object(const char* json_string);

#endif // JSON_H
