#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/adc.h"
#include "driver/i2s.h"
#include "esp_log.h"

#define ADC_CHANNEL ADC1_CHANNEL_6   // ADC1 Channel 0 is GPIO36
#define SAMPLE_RATE 44100             // 44.1 kHz sample rate for I2S
#define DMA_BUFFER_SIZE 10        // Buffer size for DMA transfers
#define ADC_MAX_VALUE 4095            // 12-bit ADC resolution (0 to 4095)

static const char *TAG = "ADC_DMA_I2S";

// ADC reading buffer
uint16_t adc_reading[DMA_BUFFER_SIZE];

// I2S configuration for ADC capture
void i2s_adc_setup(void) {
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_ADC_BUILT_IN | I2S_MODE_RX,   // Master mode, ADC input, RX mode
        .sample_rate = SAMPLE_RATE,                       // Sampling rate
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,     // 16-bit resolution
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,      // Only one channel (left)
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,  // MSB format
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,         // Interrupt level 1
        .dma_buf_count = 2,                               // Two DMA buffers
        .dma_buf_len = DMA_BUFFER_SIZE,                   // Buffer size
        .use_apll = false,                                // No APLL used
        .tx_desc_auto_clear = true,                       // Auto-clear DMA
        .fixed_mclk = 0                                   // No fixed master clock
    };

    // Install and start I2S driver
    esp_err_t ret = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install I2S driver: %s", esp_err_to_name(ret));
        return;
    }

    // Configure I2S ADC mode
    i2s_set_adc_mode(ADC_UNIT_1, ADC_CHANNEL);
    
    // Enable ADC capture via I2S
    i2s_adc_enable(I2S_NUM_0);
}

void adc_dma_read_task(void *param) {
    size_t bytes_read;

    while (1) {
        // Read ADC data via I2S (data stored in adc_reading buffer)
        esp_err_t ret = i2s_read(I2S_NUM_0, (void*) adc_reading, DMA_BUFFER_SIZE * sizeof(uint16_t), &bytes_read, portMAX_DELAY);
        
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "I2S read error: %s", esp_err_to_name(ret));
            continue; // Skip the current loop iteration
        }
        
      
        
        // Log the ADC values
        int sample_count = bytes_read / sizeof(uint16_t);  // Calculate number of samples
        for (int i = 0; i < sample_count; i++) {
            ESP_LOGI(TAG, "ADC Value[%d]: %d", i, adc_reading[i] & 0x0FFF);  // Mask to get the 12-bit value
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // 2-second delay
    }
}


void app_main(void) {
    // Configure ADC width and attenuation
    esp_err_t ret = adc1_config_width(ADC_WIDTH_BIT_12);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ADC width: %s", esp_err_to_name(ret));
        return;
    }
    
    ret = adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11); // 11 dB attenuation (0 to 3.6V)
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ADC channel: %s", esp_err_to_name(ret));
        return;
    }

    // Setup I2S and ADC
    i2s_adc_setup();
    
    // Create a FreeRTOS task to read ADC data
    xTaskCreate(adc_dma_read_task, "adc_dma_read_task", 2048, NULL, 5, NULL);
}
