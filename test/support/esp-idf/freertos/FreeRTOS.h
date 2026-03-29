#ifndef FREERTOS_STUB_H
#define FREERTOS_STUB_H

#include <stdint.h>

// Basic FreeRTOS types for testing
#define pdMS_TO_TICKS(x) ((x) / portTICK_PERIOD_MS)
#define portTICK_PERIOD_MS 1

// Task delay function - stub implementation
void vTaskDelay(uint32_t ticks);

#endif // FREERTOS_STUB_H
