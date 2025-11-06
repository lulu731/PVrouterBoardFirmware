#include "json.h"

#include "cJSON.h"

#include <stdlib.h>
#include <string.h>

char *json_stringify(gain_object* object, uint8_t size){
    cJSON *root = cJSON_CreateObject();

    for(int i = 0; i < size; i++){
        cJSON_AddNumberToObject(root, object->key, object->value);
        object++;
    }

    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_string;
}

gain_object* json_parse_gain_object(const char* json_string)
{
    cJSON *root = cJSON_Parse(json_string);
    if (cJSON_IsNumber(root->child))
    {
        gain_object* object = malloc(sizeof(gain_object));
        object->key = malloc(strlen(root->child->string) + 1);
        strncpy(object->key, root->child->string, sizeof(object->key));
        object->value = (root->child->valueint);
        cJSON_Delete(root);
        return object;
    }
}
