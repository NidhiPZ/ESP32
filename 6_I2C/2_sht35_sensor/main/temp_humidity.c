#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/i2c.h>
#include <driver/i2c_master.h>
#include <driver/i2c_slave.h>
#include <esp_log.h>

#define I2C_MASTER_SCL_IO           19     // GPIO number for I2C SCL
#define I2C_MASTER_SDA_IO           18     // GPIO number for I2C SDA
#define I2C_MASTER_NUM              I2C_NUM_0  // I2C port number
#define I2C_MASTER_FREQ_HZ          50000 // I2C clock frequency
// #define I2C_MASTER_TX_BUF_DISABLE   0      // I2C master does not need buffer
// #define I2C_MASTER_RX_BUF_DISABLE   0      // I2C master does not need buffer

#define SHT3X_SENSOR_ADDR           0x45  // I2C address of the SHT3x sensor
#define SHT3X_CMD_MEASURE_HIGHREP   0x2400 // High repeatability measurement command

static const char *TAG = "SHT3X";
    
void i2c_master_init() 
{
    i2c_config_t conf = 
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

esp_err_t sht3x_read_temp_humidity(float *temperature, float *humidity) 
{
    uint8_t write_buffer[2] = {SHT3X_CMD_MEASURE_HIGHREP >> 8, SHT3X_CMD_MEASURE_HIGHREP & 0xFF};
    uint8_t read_buffer[6];

    // Create I2C command link
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT3X_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, write_buffer, 2, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Error in writing I2C command");
        return ret;
    }

    // Delay for measurement
    vTaskDelay(50 / portTICK_PERIOD_MS);

    // Read the sensor data
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT3X_SENSOR_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, read_buffer, sizeof(read_buffer), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error in reading I2C response");
        return ret;
    }

    // Convert the data into temperature and humidity
    uint16_t raw_temp = (read_buffer[0] << 8) | read_buffer[1];
    uint16_t raw_humidity = (read_buffer[3] << 8) | read_buffer[4];

    // Formula from SHT3x datasheet for temperature and humidity
    *temperature = -45 + (175 * ((float)raw_temp / 65535.0));
    *humidity = 100 * ((float)raw_humidity / 65535.0);

    return ESP_OK;
}

void app_main()
 {
    i2c_master_init();

    float temperature, humidity;
    while (1) 
    {
        if (sht3x_read_temp_humidity(&temperature, &humidity) == ESP_OK) {
            printf("Temperature: %.2f°C, Humidity: %.2f%%\n", temperature, humidity);
        } 
        else 
        {
            printf("Failed to read from SHT3x sensor\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Read sensor every 2 seconds
    }
}
