#ifndef TASK_H
#define TASK_H

#include <stdint.h>

// Basic FreeRTOS task types for testing
#define pdMS_TO_TICKS(x) ((x) / portTICK_PERIOD_MS)
#define portTICK_PERIOD_MS 1

// Task delay function - stub implementation
void vTaskDelay(uint32_t ticks);

#endif // TASK_H
