#include <stdio.h>
#include <time.h>
#include "esp_log.h"
#include "lps22hb.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys/time.h"
#include "driver/timer.h"

#define SDA_PIN 18
#define SCL_PIN 19
#define I2C_MASTER_FREQ_HZ 100000
#define TIMER_CLK_FREQ    80000000  // Timer clock frequency is 80 MHz
#define TIMER_DIVIDER         16
#define TIMER_INTERVAL_SEC    5

static const char *TAG = "APP_MAIN";
TaskHandle_t sensor_task_handle = NULL;

// I2C initialization
esp_err_t i2c_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };
    if (i2c_param_config(I2C_NUM_0, &conf) != ESP_OK ||
        i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0) != ESP_OK) 
        {
        ESP_LOGE(TAG, "I2C initialization failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "I2C initialized.");
    return ESP_OK;
}

// Sensor reading function
void read_sensor(void) 
{
    float pressure, temperature;
    if (lps22hb_read_pressure(&pressure) == ESP_OK && lps22hb_read_temperature(&temperature) == ESP_OK) {
        ESP_LOGI(TAG, "Pressure: %.2f hPa, Temperature: %.2f °C", pressure, temperature);
    } 
    else {
        ESP_LOGE(TAG, "Failed to read sensor data.");
    }
}

// Timer ISR callback
void IRAM_ATTR timer_group_isr(void *param)
{
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
    vTaskNotifyGiveFromISR(sensor_task_handle, NULL);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
}

// Sensor task
void sensor_task(void *arg) 
{
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        read_sensor();
    }
}

// Timer initialization
void init_timer(void) {
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = TIMER_AUTORELOAD_EN,
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_SEC * (TIMER_CLK_FREQ/ TIMER_DIVIDER));
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_group_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_start(TIMER_GROUP_0, TIMER_0);
    ESP_LOGI(TAG, "Timer initialized.");
}

// Main application
void app_main(void) {
    if (i2c_init() != ESP_OK || lps22hb_init() != ESP_OK) {
        ESP_LOGE(TAG, "Initialization failed");
        return;
    }
    xTaskCreate(sensor_task, "sensor_task", 2048, NULL, 10, &sensor_task_handle);
    init_timer();

    while (true) {
        struct timeval now;
        gettimeofday(&now, NULL);
        struct tm timeinfo;
        localtime_r(&now.tv_sec, &timeinfo);
        ESP_LOGI(TAG, "RTC Time: %02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay 1 second
    }
}
