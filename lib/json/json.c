#include "json.h"

#include "cJSON.h"

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
