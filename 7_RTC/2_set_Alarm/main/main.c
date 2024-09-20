#include <stdio.h>
#include <sys/time.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "esp_sleep.h"

static const char *TAG = "TIMER_ALARM_APP";

// Define GPIO pin for LED
#define LED_GPIO_PIN GPIO_NUM_2  // Change this to your LED GPIO pin

// Function to print the current date and time
void print_datetime(void)
{
    struct timeval tv;
    struct tm timeinfo;
    char strftime_buf[64];

    // Get the current time
    gettimeofday(&tv, NULL);

    // Convert to local time
    localtime_r(&tv.tv_sec, &timeinfo);

    // Format and print the date and time
    strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    ESP_LOGI(TAG, "Current date and time: %s", strftime_buf);
}

// Function to set an alarm with deep sleep based on a timer
void set_timer_alarm(int wakeup_time_sec)
{
    // Convert wakeup time to microseconds
    int64_t wakeup_time_us = wakeup_time_sec * 1000000LL;

    // Set the wakeup time for deep sleep
    esp_sleep_enable_timer_wakeup(wakeup_time_us);
    ESP_LOGI(TAG, "Alarm set for %d seconds from now, entering deep sleep...", wakeup_time_sec);

    // Print current time before sleeping
    print_datetime();

    // Enter deep sleep
    esp_deep_sleep_start();
}

void app_main()
{
    ESP_LOGI(TAG, "Initializing Timer Alarm Application...");

    // Initialize GPIO for LED
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED_GPIO_PIN),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);

    // Check if the device woke up from deep sleep
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        ESP_LOGI(TAG, "Woke up from deep sleep");

        // Turn on the LED
        gpio_set_level(LED_GPIO_PIN, 1);

        // Wait for 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));

        // Turn off the LED
        gpio_set_level(LED_GPIO_PIN, 0);

        // Print the current date and time
        print_datetime();
    } else {
        ESP_LOGI(TAG, "Initializing deep sleep...");

        // Set an alarm to wake up after 10 seconds
        set_timer_alarm(10);  // Wake up in 10 seconds
    }
}
