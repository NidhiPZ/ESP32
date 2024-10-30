#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_err.h"

void app_main(void) { 
// Function to configure PWM (LEDC) for the LED

    // Prepare and apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = 
    {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .freq_hz          = 10000,    //4khz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Prepare and apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = 
    {
        .gpio_num       = GPIO_NUM_33,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .duty           = 0, // Start with LED off
        .hpoint         = 0
    };

 // Set channel configuration
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Set duty cycle to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 128));

    // Update the duty cycle
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));

    ESP_LOGI("PWM", "PWM started with 50%% duty cycle on GPIO %d", GPIO_NUM_33);

}
