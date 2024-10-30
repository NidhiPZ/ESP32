#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"  // New Pulse Counter API


static const char *TAG = "Pulse Counter";
#define GPIO_A 2

// Global variable for the pulse counter unit handle
pcnt_unit_handle_t pcnt_unit = NULL;

// Function to configure and initialize Pulse Counter (PCNT)
void pcnt_init(void)
{
    // Initialize pulse counter unit configuration
    pcnt_unit_config_t unit_config = {
        .high_limit = 11,   // Set upper limit
        .low_limit = -10,    // Set lower limit
    };
    
    // Create the pulse counter unit
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));  // Initialize the PCNT unit
    
    // Configure the pulse counter channel for GPIO input
    pcnt_chan_config_t chan_config = {
        .edge_gpio_num = GPIO_A, 
        .level_gpio_num = -1  // No level GPIO used
    };
     // Set glitch filter
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config);
    pcnt_channel_handle_t pcnt_channel;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_channel));  // Create channel
    
    // Set counting mode to increment on positive edge
    //ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_channel, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_channel, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));

    // Set the initial value to zero
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));

    // Enable the pulse counter unit
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));

    // Start counting
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}

void app_main(void)
{
    // Initialize Pulse Counter (PCNT)
    pcnt_init();

    int16_t pulse_count = 0;

    // Continuously check the pulse count and print it
    while (1) {
        // Get the current pulse count from PCNT unit
        ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));

        // Print the current pulse count
        ESP_LOGI(TAG, "Pulse count: %d", pulse_count);

        // If the pulse count reaches the upper limit, reset the counter
        if (pulse_count == 10) {
            ESP_LOGW(TAG, "Pulse count reached upper limit, resetting counter...");
            ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));  // Clear the counter
        }

        // Add a small delay for readability
        vTaskDelay((100));
    }
}
