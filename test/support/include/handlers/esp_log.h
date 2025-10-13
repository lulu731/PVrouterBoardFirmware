#ifndef ESP_LOG_H
#define ESP_LOG_H

extern int ESP_LOGE_called;

#define ESP_LOGE(tag, format, ...) ESP_LOGE_called++

#define ESP_LOGI(tag, format, ...)

#endif // ESP_LOG_H
