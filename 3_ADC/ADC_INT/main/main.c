#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc/adc_continuous.h"
#include "esp_log.h"

#define ADC_CHANNEL ADC1_CHANNEL_6   // ADC1 Channel 6 corresponds to GPIO34
#define SAMPLE_FREQ_HZ 1000          // Sampling frequency

static const char *TAG = "POT_ADC_INTERRUPT";

#define BUFFER_SIZE 1024

// Callback function when ADC buffer is full
static void adc_continuous_handler(void *arg)
{
    uint8_t buffer[BUFFER_SIZE];
    uint32_t bytes_read = 0;

    esp_err_t ret = adc_continuous_read((adc_continuous_handle_t)arg, buffer, BUFFER_SIZE, &bytes_read, portMAX_DELAY);
    if (ret == ESP_OK) {
        for (int i = 0; i < bytes_read; i += 4) {  // Assuming 12-bit data
            uint32_t adc_value = ((uint32_t)buffer[i + 1] << 8) | buffer[i];
            ESP_LOGI(TAG, "Potentiometer Value: %ld", adc_value);
        }
    } else {
        ESP_LOGE(TAG, "Failed to read ADC data");
    }
}

void app_main(void)
{
    // ADC continuous configuration
    adc_continuous_handle_t handle;

    adc_continuous_config_t adc_config = {
        .pattern_num = 1,
        .sample_freq_hz = SAMPLE_FREQ_HZ,         // Sampling frequency
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1    // Output format
    };

    // Initialize ADC pattern configuration
    adc_digi_pattern_config_t adc_pattern[1] = {
        {
            .atten = ADC_ATTEN_DB_0,            // No attenuation (input range: 0-1.1V)
            .bit_width = ADC_BITWIDTH_DEFAULT,  // Default bit width (usually 12 bits)
            .channel = ADC_CHANNEL,             // ADC1 Channel 6 (GPIO34)
            .unit = ADC_UNIT_1
        }
    };

    // Attach pattern to configuration
    adc_config.adc_pattern = adc_pattern;
    adc_config.pattern_num = sizeof(adc_pattern) / sizeof(adc_pattern[0]);

    // Initialize ADC continuous driver
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    // Start ADC continuous conversion
    ESP_ERROR_CHECK(adc_continuous_start(handle));

    // Create a task to handle ADC data
    xTaskCreate(adc_continuous_handler, "adc_handler", 4096, (void*)handle, 5, NULL);

    // Main loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Keep main task running
    }
}
