#include "json.h"

#include "cJSON.h"

#include <stdlib.h>
#include <string.h>

gain_object null_gain_object = {NULL, 0};

char *json_stringify(gain_object* gain_object, uint8_t size){
    cJSON *root = cJSON_CreateObject();
    cJSON *objects = cJSON_AddArrayToObject(root, "objects");

    for(int i = 0; i < size; i++){
        cJSON *object = cJSON_CreateObject();
        cJSON_AddStringToObject(object, "id", gain_object->key);
        cJSON_AddNumberToObject(object, "value", gain_object->value);
        cJSON_AddItemToArray(objects, object);
        gain_object++;
    }

    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_string;
}

gain_object* json_parse_gain_object(const char* json_string)
{
    cJSON *root = cJSON_Parse(json_string);
    if (root && cJSON_IsArray(root->child))
    {
        cJSON *json_array = cJSON_GetObjectItemCaseSensitive(root, "objects");
        cJSON *json_object = json_array->child;
        cJSON *json_id = cJSON_GetObjectItemCaseSensitive(json_object, "id");
        cJSON *json_value = cJSON_GetObjectItemCaseSensitive(json_object, "value");

        gain_object* object = malloc(sizeof(gain_object));
        object->key = malloc(strlen(json_id->valuestring) + 1);
        strncpy(object->key, json_id->valuestring, sizeof(object->key));
        object->value = (json_value->valueint);

        cJSON_Delete(root);

        return object;
    }
    cJSON_Delete(root);
    return &null_gain_object;
}
