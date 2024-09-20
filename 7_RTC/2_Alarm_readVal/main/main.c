#include <stdio.h>
#include "lps22hb.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"

#define SDA_PIN 18
#define SCL_PIN 19
#define I2C_MASTER_FREQ_HZ 100000  // 100kHz I2C frequency

static const char *TAG = "APP_MAIN";

// I2C initialization function
esp_err_t i2c_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };

    esp_err_t err = i2c_param_config(I2C_NUM_0, &conf);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "I2C param config failed");
        return err;
    }

    err = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "I2C driver install failed");
    }

    ESP_LOGI(TAG, "I2C initialized.");
    return err;
}

// Main application
void app_main(void)
{
    // Initialize I2C
    if (i2c_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        return;
    }

    // Initialize sensor
    if (lps22hb_init() != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to initialize LPS22HB sensor");
        return;
    }

    // Loop to read pressure every 5 seconds with countdown
    while (true) 
    {
        // Countdown display for 5 seconds
        for (int i = 5; i > 0; i--) 
        {
            ESP_LOGI(TAG, "Countdown: %d", i);
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
        }

        // Read pressure once after countdown
        float pressure = 0;
        if (lps22hb_read_pressure(&pressure) != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to read pressure.");
        }

        // Wait for the remainder of the 5 seconds before the next countdown
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for 5 seconds before the next cycle
    }
}