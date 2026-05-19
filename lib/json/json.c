#include "json.h"

#include "cJSON.h"

#include <stdlib.h>
#include <string.h>

gain_object null_gain_object = {NULL, 0};

/**
 * @brief converts an array of gain_object structs to a JSON string
 * @param gain_object an array of gain_object structs
 * @param size the number of elements in the array
 * @return a JSON string representing the array of gain_object structs
 */
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

/**
 * @brief parses a JSON string representing an array of gain_object structs
 * @param json_string a JSON string representing an array of gain_object structs
 * @return a gain_object struct representing the first element in the array
 */
gain_object* json_parse_gain_object(const char* json_string)
{
    cJSON *root = cJSON_Parse(json_string);

    cJSON *json_array = NULL;
    cJSON *json_object = NULL;
    cJSON *json_id = NULL;
    cJSON *json_value = NULL;

    if (root && cJSON_IsArray(root->child))
    {
        json_array = cJSON_GetObjectItemCaseSensitive(root, "objects");
        if (json_array && cJSON_IsArray(json_array))
        {
            // Get the first object in the array
            json_object = cJSON_GetArrayItem(json_array, 0);
            if (json_object)
            {
                json_id = cJSON_GetObjectItemCaseSensitive(json_object, "id");
                json_value = cJSON_GetObjectItemCaseSensitive(json_object, "value");
            }
        }
    }

    gain_object* object = NULL;
    if (json_id && json_value)
    {
        object = malloc(sizeof(gain_object));
        object->key = malloc(strlen(json_id->valuestring) + 1);
        strncpy(object->key, json_id->valuestring, strlen(json_id->valuestring) + 1);
        object->value = (json_value->valueint);
    }
    else
        object = &null_gain_object;

    cJSON_Delete(root);
    return object;
}
