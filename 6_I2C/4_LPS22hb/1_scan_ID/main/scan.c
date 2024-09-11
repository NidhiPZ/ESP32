#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include "sdkconfig.h"

#define SDA_PIN 18
#define SCL_PIN 19
#define SENSOR_ID_REG 0x0F  // Change this if your sensor uses a different register for ID

static char tag[] = "i2cscanner";

void task_i2cscanner(void *ignore) 
{
    ESP_LOGD(tag, ">> i2cScanner");
    
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

            // Read the sensor ID from SENSOR_ID_REG
            uint8_t sensor_id;
            i2c_cmd_handle_t id_cmd = i2c_cmd_link_create();
            i2c_master_start(id_cmd);
            i2c_master_write_byte(id_cmd, (i << 1) | I2C_MASTER_WRITE, true);
            i2c_master_write_byte(id_cmd, SENSOR_ID_REG, true);
            i2c_master_start(id_cmd);
            i2c_master_write_byte(id_cmd, (i << 1) | I2C_MASTER_READ, true);
            i2c_master_read_byte(id_cmd, &sensor_id, I2C_MASTER_LAST_NACK);
            i2c_master_stop(id_cmd);
            espRc = i2c_master_cmd_begin(I2C_NUM_0, id_cmd, 10 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(id_cmd);

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