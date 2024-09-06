#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/adc.h>
#include <esp_log.h>
#include <time.h>

// Tag for logging
static const char *TAG = "ADC_SIMULATION";

// Simulated ADC read function
int simulate_adc_read()
{
    // Return a simulated ADC value between 0 and 4095 (12-bit resolution)
    return rand() % 4096;
}

void adc_task(void *param)
{
    while (1)
    {
        // Simulate ADC reading
        int adc_value = simulate_adc_read();

        // Log the simulated ADC value
        ESP_LOGI(TAG, "Simulated ADC Value: %d", adc_value);

        // Delay for 1 second to simulate periodic ADC sampling
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1000 ms delay, adjust as needed
    }
}

void app_main(void)
{
    // Seed the random number generator
    srand((unsigned) time(NULL));

    // Create a task to simulate ADC readings periodically
    xTaskCreate(adc_task, "adc_task", 2048, NULL, 5, NULL);
}
