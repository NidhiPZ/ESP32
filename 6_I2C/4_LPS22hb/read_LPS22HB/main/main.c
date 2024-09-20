<<<<<<< HEAD
#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include "LPS22HB.h"

static const char *TAG = "APP_MAIN";
#define SDA_PIN 18
#define SCL_PIN 19

// I2C initialization
esp_err_t i2c_init()
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);
    return i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

// Read and display sensor data
void read_lps22hb_data(void *arg)
{
    float pressure, temperature;
    
    while (1)
    {
        if (lps22hb_read_pressure(&pressure) == ESP_OK && lps22hb_read_temperature(&temperature) == ESP_OK)
        {
            ESP_LOGI(TAG, "Pressure: %.2f hPa, Temperature: %.2f C", pressure, temperature);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read sensor data");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay 1 second between readings
    }
}

void app_main(void)
{
    esp_err_t ret;

    // Initialize I2C
    i2c_init();

    // Verify WHO_AM_I register
    ret = lps22hb_who_am_i(LPS22HB_ADDRESS);
    if (ret == ESP_OK)
    {
        // WHO_AM_I matched, initialize the sensor
        lps22hb_init();

        // Start reading sensor data
        xTaskCreate(read_lps22hb_data, "read_lps22hb_data", 4096, NULL, 10, NULL);
    }
    else
    {
        ESP_LOGE(TAG, "WHO_AM_I check failed. Sensor not recognized.");
    }
}
=======
#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include "LPS22HB.h"

static const char *TAG = "APP_MAIN";
#define SDA_PIN 18
#define SCL_PIN 19

// I2C initialization
esp_err_t i2c_init()
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);
    return i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

// Read and display sensor data
void read_lps22hb_data(void *arg)
{
    float pressure, temperature;
    
    while (1)
    {
        if (lps22hb_read_pressure(&pressure) == ESP_OK && lps22hb_read_temperature(&temperature) == ESP_OK)
        {
            ESP_LOGI(TAG, "Pressure: %.2f hPa, Temperature: %.2f C", pressure, temperature);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read sensor data");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay 1 second between readings
    }
}

void app_main(void)
{
    esp_err_t ret;

    // Initialize I2C
    i2c_init();

    // Verify WHO_AM_I register
    ret = lps22hb_who_am_i(LPS22HB_ADDRESS);
    if (ret == ESP_OK)
    {
        // WHO_AM_I matched, initialize the sensor
        lps22hb_init();

        // Start reading sensor data
        xTaskCreate(read_lps22hb_data, "read_lps22hb_data", 4096, NULL, 10, NULL);
    }
    else
    {
        ESP_LOGE(TAG, "WHO_AM_I check failed. Sensor not recognized.");
    }
}
>>>>>>> myorigin/master
