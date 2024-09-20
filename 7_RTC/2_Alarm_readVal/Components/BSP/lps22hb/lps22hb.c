#include "lps22hb.h"
#include "esp_log.h"

static const char *TAG = "LPS22HB";

// I2C write helper
static esp_err_t i2c_write(uint8_t reg, uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LPS22HB_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// I2C read helper
static esp_err_t i2c_read(uint8_t reg, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LPS22HB_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LPS22HB_ADDRESS << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Check WHO_AM_I register
esp_err_t lps22hb_who_am_i(void)
{
    uint8_t who_am_i = 0;
    esp_err_t ret = i2c_read(LPS22HB_WHO_AM_I_REG, &who_am_i, 1);

    if (ret == ESP_OK && who_am_i == LPS22HB_WHO_AM_I_RESPONSE) {
        ESP_LOGI(TAG, "WHO_AM_I check passed.");
        return ESP_OK;
    } else {
        ESP_LOGE(TAG, "WHO_AM_I check failed. Expected 0xB1, got 0x%02X", who_am_i);
        return ESP_FAIL;
    }
}

// Initialize the sensor
esp_err_t lps22hb_init(void)
{
    return lps22hb_who_am_i();
}

// Read pressure data
esp_err_t lps22hb_read_pressure(float *pressure)
{
    uint8_t data[3];
    esp_err_t ret = i2c_read(0x28, data, 3);  // Pressure register

    if (ret == ESP_OK) {
        int32_t raw_pressure = (int32_t)((data[2] << 16) | (data[1] << 8) | data[0]);
        *pressure = raw_pressure / 4096.0;  // Convert to hPa
        ESP_LOGI(TAG, "Pressure: %.2f hPa", *pressure);
    } else {
        ESP_LOGE(TAG, "Failed to read pressure.");
    }

    return ret;
}
