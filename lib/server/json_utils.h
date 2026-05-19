#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <stdint.h>
#include "adc_registers.h"
#include "adc_rw.h"
#include "json.h"

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
                           const char* key3, uint16_t value3);

/**
 * @brief Creates a JSON message from ADC RMS values
 * @return JSON string (caller must free), or NULL on failure
 */
char* create_adc_rms_json_message(void);

#endif // JSON_UTILS_H