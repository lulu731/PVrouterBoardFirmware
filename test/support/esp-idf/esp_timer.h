#ifndef ESP_TIMER_H
#define ESP_TIMER_H

#include "esp_err.h"

typedef struct esp_timer* esp_timer_handle_t;


typedef void (*esp_timer_cb_t)(void* arg);


typedef struct {
    esp_timer_cb_t callback;        //!< Callback function to execute when timer expires
    void* arg;                      //!< Argument to pass to callback
    //esp_timer_dispatch_t dispatch_method;   //!< Dispatch callback from task or ISR; if not specified, esp_timer task
    //                                !< is used; for ISR to work, also set Kconfig option
    //                                !< `CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD`
    const char* name;               //!< Timer name, used in esp_timer_dump() function
    //bool skip_unhandled_events;     //!< Setting to skip unhandled events in light sleep for periodic timers
} esp_timer_create_args_t;


esp_err_t esp_timer_create(const esp_timer_create_args_t* create_args,
                           esp_timer_handle_t* out_handle);

esp_err_t esp_timer_start_periodic(esp_timer_handle_t timer, uint64_t period);

esp_err_t esp_timer_delete(esp_timer_handle_t timer);

esp_err_t esp_timer_stop(esp_timer_handle_t timer);

#endif // ESP_TIMER_H
