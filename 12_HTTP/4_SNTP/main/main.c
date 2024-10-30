#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"

static const char *TAG = "SNTP";
char Current_Date_Time[100];

// Replace with your Wi-Fi credentials
#define WIFI_SSID "Conference"
#define WIFI_PASS "DNKASDFGHJKL"

// Define the alarm time (24-hour format)
int alarm_hour = 15;  // Set the alarm hour (e.g., 14 for 2:00 PM)
int alarm_minute = 46; // Set the alarm minute (e.g., 35 for 2:35 PM)

// Callback function to notify when time is synchronized
void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Time synchronized");
}

void Get_current_date_time(char *date_time) {
    char strftime_buf[64];
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    // Set timezone to UTC + 5:30 (IST)
    setenv("TZ", "UTC-05:30", 1);
    tzset();
    localtime_r(&now, &timeinfo);

    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time: %s", strftime_buf);
    strcpy(date_time, strftime_buf);
}

// Function to check if the current time matches the alarm time
void check_alarm() {
    while (true) {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);

        if (timeinfo.tm_hour == alarm_hour && timeinfo.tm_min == alarm_minute) {
            ESP_LOGI(TAG, "Alarm Triggered! Current time: %02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
            break;
        }
        ESP_LOGI(TAG, "Waiting for alarm... Current time: %02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
        vTaskDelay(60000 / portTICK_PERIOD_MS); // Check every minute
     }
    vTaskDelete(NULL);
}

// Function to initialize SNTP
void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);         // Polling mode
    sntp_setservername(0, "pool.ntp.org");           // Use NTP server pool
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

// Function to obtain time
void obtain_time(void)
{
    initialize_sntp();

    // Wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (1970 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 2 seconds
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == retry_count) {
        ESP_LOGE(TAG, "Failed to get time");
    }
}

// Function to connect to Wi-Fi
void wifi_init(void)
{
    esp_netif_init();                               // Initialize network stack
    esp_event_loop_create_default();                // Create event loop
    esp_netif_create_default_wifi_sta();            // Create default Wi-Fi station

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);                            // Initialize Wi-Fi

    esp_wifi_set_mode(WIFI_MODE_STA);               // Set as station mode
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config); // Set Wi-Fi configuration
    esp_wifi_start();                               // Start Wi-Fi
    esp_wifi_connect();                             // Connect to Wi-Fi
}

// Main application
void app_main(void)
{
    ESP_LOGI(TAG, "ESP32 SNTP Example");

    // Initialize NVS (non-volatile storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    wifi_init(); // Initialize Wi-Fi

    // Obtain time via SNTP
    obtain_time();

    Get_current_date_time(Current_Date_Time);
    ESP_LOGI(TAG, "Current Date and Time: %s", Current_Date_Time);

    // Start the alarm check task
    xTaskCreate(check_alarm, "check_alarm", 2048, NULL, 5, NULL);
}
