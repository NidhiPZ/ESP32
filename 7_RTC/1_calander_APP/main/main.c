#include <stdio.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "esp_system.h"
#include "sys/time.h"

static const char *TAG = "CALENDAR_APP";

// Function to set the internal RTC time
void set_internal_time(struct tm *timeinfo)
{
    // Convert tm structure to time_t
    time_t now = mktime(timeinfo);

    // Set the time using the internal RTC
    struct timeval tv = {
        .tv_sec = now,
        .tv_usec = 0
    };
    settimeofday(&tv, NULL);
}

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
    strftime(strftime_buf, sizeof(strftime_buf), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    ESP_LOGI(TAG, "Current date and time: %s", strftime_buf);
}

// Main task
void calendar_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Calendar Application started...");

    // Initialize the RTC with a specific date and time
    struct tm initial_time = {
        .tm_year = 2024- 1900, // Year since 1900 (2021)
        .tm_mon  = 9 - 1,      // Month (0-based, so 1 = January)
        .tm_mday = 19,
        .tm_hour = 16,
        .tm_min  = 30,
        .tm_sec  = 0,
        .tm_isdst = -1        // No Daylight Saving Time
    };

    set_internal_time(&initial_time);

    while (1)
    {
        print_datetime();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}

void app_main()
{
    ESP_LOGI(TAG, "Initializing calendar application...");

    // Create a task to manage the calendar
    xTaskCreate(calendar_task, "calendar_task", 1024 * 8, NULL, 5, NULL);
}
