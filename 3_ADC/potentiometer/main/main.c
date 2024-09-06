#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/adc.h>
#include <esp_log.h>
#include <stdint.h>

void app_main(void)
{
    // Configure ADC width
    adc1_config_width(ADC_WIDTH_BIT_12);

    // Configure ADC channel and attenuation
    adc2_config_channel_atten(ADC2_CHANNEL_2, ADC_ATTEN_DB_11);

    while (1) 
    {
        int adc_reading = 0;

        // Get ADC value
        esp_err_t ret = adc2_get_raw(ADC2_CHANNEL_2, ADC_WIDTH_BIT_12, &adc_reading);
        if (ret == ESP_OK) 
        {
            // Print the ADC value
            printf("ADC Reading: %d\n", adc_reading);
        } else {
            printf("ADC Read Error: %d\n", ret);
        }

        vTaskDelay(1000/portTICK_PERIOD_MS); // Delay for 1 second
    }
}
