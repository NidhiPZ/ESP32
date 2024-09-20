#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include "sdkconfig.h"

#define SDA_PIN 18
#define SCL_PIN 19
#define SENSOR_ID_REG 0x0F  // WHO_AM_I register default

static char tag[] = "i2cscanner";

// Function to read the WHO_AM_I register
esp_err_t who_am_i(uint8_t device_addr, uint8_t *sensor_id) 
{
    esp_err_t espRc;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, SENSOR_ID_REG, true);    // Send the WHO_AM_I register address
    i2c_master_start(cmd);   // Repeated start to switch to read mode
    // Send device address with read flag
    i2c_master_write_byte(cmd, (device_addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, sensor_id, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    // Send the command link and get the result
    espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return espRc;
}

void task_i2cscanner(void *ignore) 
{
    ESP_LOGD(tag, ">> i2cScanner");
    
    // I2C configuration
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.scl_io_num = SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
    printf("00:         ");
    for (int i = 3; i < 0x78; i++) 
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);

        esp_err_t espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (i % 16 == 0) 
        {
            printf("\n%.2x:", i);
        }
        if (espRc == ESP_OK) 
        {
            printf(" 0x%.2x", i);

            // Read the sensor ID using the who_am_i function
            uint8_t sensor_id;
            espRc = who_am_i(i, &sensor_id);
            if (espRc == ESP_OK) 
            {
                printf(" (ID: 0x%.2x)", sensor_id);
            } 
            else 
            {
                printf(" (Failed to read ID)");
            }
        } 
        else 
        {
            printf(" --");
        }
    }
    printf("\n");
    printf("I2C scan completed.\n");
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(task_i2cscanner, "scanner_task", 4096, NULL, 10, NULL);
}
