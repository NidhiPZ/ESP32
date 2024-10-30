#include "esp_sleep.h"
#include "esp_log.h"

#define SLEEP_DURATION_US 20 * 1000000 // 5 seconds

void app_main() {
    ESP_LOGI("Power Management", "Entering Deep Sleep Mode...");

    // Set up the timer wake-up for deep sleep
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION_US);

    // Enter deep sleep
    esp_deep_sleep_start();
}
