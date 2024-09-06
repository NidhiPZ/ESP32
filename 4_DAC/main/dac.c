#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/dac.h>
#include <math.h>

#define DAC_CHANNEL DAC_CHANNEL_1 // Using DAC1 (GPIO25)
#define WAVEFORM_STEP 8           // Step size for sawtooth waveform
#define WAVEFORM_DELAY 10         // Delay in ms between steps

void dac_waveform_task(void *param)
{
    uint8_t value = 0; // DAC output value (0-255)

    while (1)
    {
        // Output the value to the DAC
        dac_output_voltage(DAC_CHANNEL, value);

        // Increment the value to create a scdawtooth wave
        value += WAVEFORM_STEP;

        // Reset to 0 when maximum value is reached
        if (value >= 256)
        {
            value = 0;
        }

        // Delay to control the waveform frequency
        vTaskDelay(pdMS_TO_TICKS(WAVEFORM_DELAY));
    }
}

void app_main(void)
{
    // Initialize the DAC channel
    dac_output_enable(DAC_CHANNEL);

    // Create a task to generate the waveform
    xTaskCreate(dac_waveform_task, "dac_waveform_task", 2048, NULL, 5, NULL);
}
