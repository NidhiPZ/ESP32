#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include  "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char* TAG = "TIMER_EXAMPLE";

void timer_callback(void* arg) 
{
    ESP_LOGI(TAG, "Timer run periodically");
}

void app_main(void) {
    const esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
        .name = "example_timer"
    };

    // Create the timer
    esp_timer_handle_t timer;
    esp_timer_create(&timer_args, &timer);

    // Start the timer with a 1-second interval (1000000 microseconds)
    esp_timer_start_periodic(timer, 1000000);

    // Let the timer run for 5 seconds before stopping and deleting it
    vTaskDelay(pdMS_TO_TICKS(5000));
    esp_timer_stop(timer);
    esp_timer_delete(timer);

    ESP_LOGI(TAG, "Timer stopped and deleted");
}
