#include "json_utils.h"
#include "json.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a JSON message from three key-value pairs
 * @param key1 First key name
 * @param value1 First value
 * @param key2 Second key name
 * @param value2 Second value
 * @param key3 Third key name
 * @param value3 Third value
 * @return JSON string (caller must free), or NULL on failure
 */
char* create_json_message_3(const char* key1, uint16_t value1,
                           const char* key2, uint16_t value2,
                           const char* key3, uint16_t value3)
{
    gain_object objects[3] = {
        {key1, value1},
        {key2, value2},
        {key3, value3}
    };
    return json_stringify(objects, 3);
}

/**
 * @brief Creates a JSON message from ADC RMS values
 * @return JSON string (caller must free), or NULL on failure
 */
char* create_adc_rms_json_message(void)
{
    // Read ADC registers for RMS values
    read_adc_register(&U_RMS);
    read_adc_register(&I_RMS);
    read_adc_register(&I_RMS_2);

    // Create JSON with RMS values
    return create_json_message_3("U_RMS", U_RMS.data,
                               "I_RMS", I_RMS.data,
                               "I_RMS_2", I_RMS_2.data);
}