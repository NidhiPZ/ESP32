#include <stdio.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BOOT_BUTTON GPIO_NUM_0  // Boot button pin
#define LED_PIN GPIO_NUM_2      // LED pin

RTC_DATA_ATTR int wakeup_count = 0; // Wake-up count stored in RTC memory


void app_main(void)
{
    // Configure the boot button GPIO  
    gpio_set_direction(BOOT_BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOOT_BUTTON, GPIO_PULLUP_ONLY); // Enable internal pull-up

    // Configure LED GPIO
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT); // Set LED pin as output

    // Print initial message
    printf("Press the boot button to wake up...\n");

    // Increase the wakeup count
    wakeup_count++;
    printf("Wakeup count: %d\n", wakeup_count);

    gpio_set_level(LED_PIN, 1); // Turn LED on
    printf("led on\n");

    vTaskDelay((5000)/portTICK_PERIOD_MS);

    gpio_set_level(LED_PIN, 0); // Turn LED off
    printf("led off\n");

    // Configure wake-up on button press
    esp_sleep_enable_ext0_wakeup(BOOT_BUTTON, 0); // Wake up when the button is pressed (0 = low)

    // Go to sleep
    vTaskDelay((1000)/portTICK_PERIOD_MS);

    printf("Going to sleep now...\n");
    esp_deep_sleep_start();

    // This part should not be reached unless waking up
    printf("Woken up!\n");
}
