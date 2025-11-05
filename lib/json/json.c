#include "json.h"

#include "cJSON.h"

char *json_stringify(const char *key, uint16_t value)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, key, value);
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_string;
}
